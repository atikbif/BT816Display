/*
 * can.c
 *
 *  Created on: 2023 Mar 11
 *      Author: Roman
 */

#include "can.h"
#include "at32f435_437_board.h"
#include <string.h>
#include "plc_data.h"
#include "cluster_state.h"

extern cluster cl;

enum Service_type {SRV_Channel, SRV_Heartbeat, SRV_Download, SRV_Spare, SRV_Write, SRV_Read, SRV_Action, SRV_Event};
enum SS_type {SS_Req_not_fragm, SS_Req_fragm, SS_Success_resp_not_fragm, SS_Success_resp_fragm, SS_Err_resp_not_fragm, SS_Err_resp_fragm, SS_Wait_resp_not_fragm, SS_Wait_resp_fragm};

enum ExtCmd {ExtHeartbeat, ExtResponse, ExtGetInfo, ExtDOState, ExtDOWrite, ExtAIState, ExtByteWrite,
			 ExtWriteConf, ExtRegsStateWithValidation, ExtRegsValidation};
enum ExtDir {BroadcastReq, ToNodeReq};
enum ExtFB {NoAnswer, RequireAnswer};
enum ExtModType {Ext_PC21, Ext_AI_Mod, Ext_DO_Mod, Ext_RS_Mod};

uint16_t hb_cnt = 0;

uint8_t cluster_addr = 0;

extern enum DI_STATE plc_di_state[DI_CNT];
extern enum DO_STATE plc_do_state[DO_CNT];
extern enum DI_TYPE plc_di_type[DI_CNT];
extern enum AI_TYPE plc_ai_type[AI_CNT];

extern uint16_t plc_ain_raw[AI_CNT];
extern uint8_t plc_clust_bits[CLUST_BITS_CNT];
extern uint16_t plc_clust_regs[CLUSTER_REGS_CNT];
extern uint8_t plc_net_bits[NET_BITS_CNT];
extern uint16_t plc_net_regs[NET_REGS_CNT];

uint16_t mask = 0;

extern uint8_t plc_can_link;
extern uint8_t plc_can_addr;
extern uint16_t plc_app_id;
extern uint8_t plc_eth_ip_state;

static uint8_t start_bit = 0;
static uint8_t packed_state = 0;
static uint8_t packed_fault = 0;
static uint8_t packed_mask = 0;

uint8_t get_io_names_flag = 0;


struct can_packet_id {
	uint16_t srv: 3;
	uint16_t node_addr: 4;
	uint16_t clust_addr: 3;
	uint16_t dir: 1;
};

struct can_packet_ext_id {
	uint16_t srv: 3;		// always SRV_Channel
	uint16_t mod_type: 6;	// module type (sender)
	uint16_t dir: 1;		// direction (1 - to address, 0 - from address)
	uint16_t req: 1;		// requires an answer
};

#define CAN_DATA_SIZE	8
#define CAN_HEADER_LENGTH	2		// SS EOID INTERN_ADDR
#define CAN_EXT_HEADER_LENGTH	2		// addr, packet signature, num of subpacket (4 bits) and quantity of packets (4 bits), cmd

struct can_packet {
	uint16_t id;
	uint8_t length;
	uint8_t data[CAN_DATA_SIZE];
};

uint16_t can1_rx_tmr = 0;
uint16_t can1_tx_tmr = 0;

static uint8_t is_packet_extended(uint32_t can_id) {
	if((can_id & 0x07) == SRV_Channel) return 1;
	return 0;
}

static enum SS_type get_ss(struct can_packet *packet) {
	return packet->data[0] >> 5;
}

static uint8_t get_eoid(struct can_packet *packet) {
	return packet->data[0] & 0b00011111;
}

static uint8_t get_intern_addr(struct can_packet *packet) {
	return packet->data[1];
}

void check_can_rx_data(can_rx_message_type *rx) {
	static struct can_packet rx_packet;
	rx_packet.id = rx->standard_id & 0xFFFF;
	rx_packet.length = rx->dlc & 0xFF;
	memcpy(rx_packet.data,rx->data,CAN_DATA_SIZE);
	if(is_packet_extended(rx->standard_id)) {

	}else {
		enum SS_type ss = get_ss(&rx_packet);
		uint8_t eoid = get_eoid(&rx_packet);
		uint8_t intern_addr = get_intern_addr(&rx_packet);
		struct can_packet_id *can_id = (struct can_packet_id *)(&rx_packet.id);
		if(can_id->srv==SRV_Heartbeat) {
			hb_cnt = 0;
			if(plc_can_link) {
				get_io_names_flag = 1;
			}
			plc_can_link = 1;
			plc_can_addr = can_id->node_addr;
			cluster_addr = can_id->clust_addr;
			cl.pc21.heartbeat = intern_addr;
		}else /*if(can_id->srv==SRV_Event)*/ {
			switch(eoid) {
			case 0x01:	// di do
				start_bit = intern_addr;
				packed_state = rx_packet.data[2];
				packed_fault = rx_packet.data[3];
				packed_mask = rx_packet.data[4];
				for(uint8_t i=0;i<8;i++) {
					if(packed_mask & (1<<i)) {
						if((start_bit+i>=1) && (start_bit+i<=DI_CNT)) {
							plc_di_type[start_bit+i-1] = DI_USED;
							cl.pc21.din[start_bit+i-1].used = 1;
							if(packed_fault & (1<<i)) {
								plc_di_state[start_bit+i-1] = DI_FAULT;
								cl.pc21.din[start_bit+i-1].fault = 1;
								cl.pc21.din[start_bit+i-1].state = 0;
							}
							else {
								cl.pc21.din[start_bit+i-1].fault = 0;
								if(packed_state & (1<<i)) {
									plc_di_state[start_bit+i-1] = DI_ON;
									cl.pc21.din[start_bit+i-1].state = 1;
								}
								else {
									plc_di_state[start_bit+i-1] = DI_OFF;
									cl.pc21.din[start_bit+i-1].state = 0;
								}
							}
						}else if((start_bit+i>=129) && (start_bit+i-129<DO_CNT)) {
							if(packed_fault & (1<<i)) {
								plc_do_state[start_bit+i-129] = DO_FAULT;
								cl.pc21.dout[start_bit+i-129].fault = 1;
							}
							else {
								cl.pc21.dout[start_bit+i-129].fault = 0;
								if(packed_state & (1<<i)) {
									plc_do_state[start_bit+i-129] = DO_ON;
									cl.pc21.dout[start_bit+i-129].state = 1;
								}
								else {
									plc_do_state[start_bit+i-129] = DO_OFF;
									cl.pc21.dout[start_bit+i-129].state = 0;
								}
							}
						}
					}
				}
				break;
			case 0x03:
				packed_state = rx_packet.data[2];
				packed_mask = rx_packet.data[3];

				for(uint8_t i=0;i<8;i++) {
					if((intern_addr+i-16)<CLUST_BITS_CNT) {
						if(packed_mask&(1<<i)) {
							if(packed_state&(1<<i)) {
								plc_clust_bits[intern_addr+i-16] = 1;
								cl.cluster_bits[intern_addr+i-16] = 1;
							}else {
								plc_clust_bits[intern_addr+i-16] = 0;
								cl.cluster_bits[intern_addr+i-16] = 0;
							}
						}
					}
				}
				break;
			case 0x05:	// ai
				if((intern_addr>=1) &&(intern_addr<=AI_CNT)) {
					plc_ain_raw[intern_addr-1] = ((uint16_t)rx_packet.data[7]<<8)|rx_packet.data[6];
					cl.pc21.ain[intern_addr-1].raw = ((uint16_t)rx_packet.data[7]<<8)|rx_packet.data[6];
					cl.pc21.ain[intern_addr-1].fault = ((uint16_t)rx_packet.data[5]<<8)|rx_packet.data[4];
					cl.pc21.ain[intern_addr-1].tdu = rx_packet.data[3];
					cl.pc21.ain[intern_addr-1].value = rx_packet.data[2];
				}
				break;
			case 0x06:
				if((intern_addr-17)<CLUSTER_REGS_CNT) {
					plc_clust_regs[intern_addr-17] = ((uint16_t)rx_packet.data[3]<<8)|rx_packet.data[2];
					cl.cluster_regs[intern_addr-17] = ((uint16_t)rx_packet.data[3]<<8)|rx_packet.data[2];
				}
				break;
			case 0x0A:
				mask = ((uint16_t)rx_packet.data[3]<<8)|rx_packet.data[2];
				if(rx_packet.data[4]==0) {
					for(uint8_t i=0;i<16;i++) {
						if(i<AI_CNT) {
							if(mask & (1<<i)) {
								plc_ai_type[i] = AI_U;
								cl.pc21.ain[i].used = 1;
							}
							else {
								plc_di_type[i] = AI_UNUSED;
								cl.pc21.ain[i].used = 0;
							}
						}
					}
				}else if(rx_packet.data[4]==1) {
					for(uint8_t i=0;i<16;i++) {
						if(i<DI_CNT) {
							if(mask & (1<<i)) {
								plc_di_type[i] = DI_USED;
								cl.pc21.din[i].used = 1;
							}else {
								plc_di_type[i] = DI_UNUSED;
								cl.pc21.din[i].used = 0;
							}
						}
					}
				}
				break;
			case 0x0E:
				intern_addr = intern_addr - (can_id->clust_addr*16) - 1;
				packed_state = rx_packet.data[2];
				packed_mask = rx_packet.data[3];

				for(uint8_t i=0;i<8;i++) {
					if((intern_addr+i)<NET_BITS_CNT) {
						if(packed_mask&(1<<i)) {
							if(packed_state&(1<<i)) {
								plc_net_bits[intern_addr+i] = 1;
								cl.net_bits[intern_addr+i] = 1;
							}else {
								plc_net_bits[intern_addr+i] = 0;
								cl.net_bits[intern_addr+i] = 0;
							}
						}
					}
				}
				break;
			case 0x0F:
				intern_addr = intern_addr - (can_id->clust_addr*16) - 1;
				if(intern_addr<NET_REGS_CNT) {
					plc_net_regs[intern_addr-1] = ((uint16_t)rx_packet.data[3]<<8)|rx_packet.data[2];
					cl.net_regs[intern_addr-1] = ((uint16_t)rx_packet.data[3]<<8)|rx_packet.data[2];
				}
				break;
			case 0x1F:
				if(intern_addr==3) {
					plc_app_id = (rx_packet.data[2] | ((uint16_t)rx_packet.data[3]<<8));
					cl.pc21.app_id = (rx_packet.data[2] | ((uint16_t)rx_packet.data[3]<<8));
				}else if(intern_addr==17) {
					plc_eth_ip_state = rx_packet.data[2];
					cl.pc21.ethip_state = rx_packet.data[2];
				}
				break;
			}
		}
	}
}

void send_heartbeat() {
	static uint8_t heartbeat_value = 1;

	uint8_t can_addr = 0x06;

	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 0x0400 | 0x01 | (can_addr<<3) | (cluster_addr << 7);
	tx_message_struct.extended_id = 0;
	tx_message_struct.id_type = CAN_ID_STANDARD;
	tx_message_struct.frame_type = CAN_TFT_DATA;
	tx_message_struct.dlc = 2;
	tx_message_struct.data[0] = 0x49;
	tx_message_struct.data[1] = heartbeat_value++;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
	//while(can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)transmit_mailbox) != CAN_TX_STATUS_SUCCESSFUL);
}

void write_clust_bit(uint16_t num, uint8_t value) {
	uint8_t can_addr = 0x06;
	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 0x0400 | 0x07 | (can_addr<<3) | (cluster_addr << 7);
	tx_message_struct.extended_id = 0;
	tx_message_struct.id_type = CAN_ID_STANDARD;
	tx_message_struct.frame_type = CAN_TFT_DATA;
	tx_message_struct.dlc = 4;
	tx_message_struct.data[0] = 0x03;
	tx_message_struct.data[1] = num+16;
	if(value) tx_message_struct.data[2] = 0x01;
	else tx_message_struct.data[2] = 0x00;
	tx_message_struct.data[3] = 0x01;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
}

void write_net_bit(uint16_t num, uint8_t value) {
	uint8_t can_addr = 0x06;
	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 0x0400 | 0x07 | (can_addr<<3) | (cluster_addr << 7);
	tx_message_struct.extended_id = 0;
	tx_message_struct.id_type = CAN_ID_STANDARD;
	tx_message_struct.frame_type = CAN_TFT_DATA;
	tx_message_struct.dlc = 4;
	tx_message_struct.data[0] = 0x0E;
	tx_message_struct.data[1] = cluster_addr*16+num+1;
	if(value) tx_message_struct.data[2] = 0x01;
	else tx_message_struct.data[2] = 0x00;
	tx_message_struct.data[3] = 0x01;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
}

void write_clust_reg(uint16_t num, uint16_t value) {
	uint8_t can_addr = 0x06;
	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 0x0400 | 0x07 | (can_addr<<3) | (cluster_addr << 7);
	tx_message_struct.extended_id = 0;
	tx_message_struct.id_type = CAN_ID_STANDARD;
	tx_message_struct.frame_type = CAN_TFT_DATA;
	tx_message_struct.dlc = 4;
	tx_message_struct.data[0] = 0x06;
	tx_message_struct.data[1] = num+17;
	tx_message_struct.data[2] = plc_clust_regs[num]&0xFF;
	tx_message_struct.data[3] = plc_clust_regs[num]>>8;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
}

void write_net_reg(uint16_t num, uint16_t value) {
	uint8_t can_addr = 0x06;
	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 0x0400 | 0x07 | (can_addr<<3) | (cluster_addr << 7);
	tx_message_struct.extended_id = 0;
	tx_message_struct.id_type = CAN_ID_STANDARD;
	tx_message_struct.frame_type = CAN_TFT_DATA;
	tx_message_struct.dlc = 4;
	tx_message_struct.data[0] = 0x0F;
	tx_message_struct.data[1] = cluster_addr*16+num+1;
	tx_message_struct.data[2] = plc_net_regs[num]&0xFF;
	tx_message_struct.data[3] = plc_net_regs[num]>>8;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
}

void can1_init(void) {
	gpio_init_type gpio_init_struct;

	can_base_type can_base_struct;
	can_baudrate_type can_baudrate_struct;
	can_filter_init_type can_filter_init_struct;

	/* enable the gpio clock */

	crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);

	gpio_default_para_init(&gpio_init_struct);

	/* configure the can tx, rx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOG, &gpio_init_struct);

	gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE0, GPIO_MUX_9);
	gpio_pin_mux_config(GPIOG, GPIO_PINS_SOURCE1, GPIO_MUX_9);

	crm_periph_clock_enable(CRM_CAN1_PERIPH_CLOCK,TRUE);

	can_default_para_init(&can_base_struct);
	can_base_struct.mode_selection = CAN_MODE_COMMUNICATE;
	can_base_struct.ttc_enable = FALSE;
	can_base_struct.aebo_enable = TRUE;
	can_base_struct.aed_enable = TRUE;
	can_base_struct.prsf_enable = FALSE;
	can_base_struct.mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;
	can_base_struct.mmssr_selection = CAN_SENDING_BY_ID;
	can_base_init(CAN1, &can_base_struct);

	/* can baudrate, set boudrate = pclk/(baudrate_div *(1 + bts1_size + bts2_size)) */
	can_baudrate_struct.baudrate_div = 72;
	can_baudrate_struct.rsaw_size = CAN_RSAW_1TQ;
	can_baudrate_struct.bts1_size = CAN_BTS1_13TQ;
	can_baudrate_struct.bts2_size = CAN_BTS2_2TQ;
	can_baudrate_set(CAN1, &can_baudrate_struct);

	/* can filter init */
	can_filter_init_struct.filter_activate_enable = TRUE;
	can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK;
	can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO0;
	can_filter_init_struct.filter_number = 0;
	can_filter_init_struct.filter_bit = CAN_FILTER_32BIT;
	can_filter_init_struct.filter_id_high = 0;
	can_filter_init_struct.filter_id_low = 0;
	can_filter_init_struct.filter_mask_high = 0;
	can_filter_init_struct.filter_mask_low = 0;
	can_filter_init(CAN1, &can_filter_init_struct);

	/* can interrupt config */
	nvic_irq_enable(CAN1_SE_IRQn, 0x00, 0x00);
	nvic_irq_enable(CAN1_RX0_IRQn, 0x00, 0x00);
	can_interrupt_enable(CAN1, CAN_RF0MIEN_INT, TRUE);

	/* error interrupt enable */
	can_interrupt_enable(CAN1, CAN_ETRIEN_INT, TRUE);
	can_interrupt_enable(CAN1, CAN_EOIEN_INT, TRUE);

	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_3 | GPIO_PINS_4;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOG, &gpio_init_struct);
	gpio_bits_write(GPIOG,GPIO_PINS_3,FALSE);
	gpio_bits_write(GPIOG,GPIO_PINS_4,FALSE);
}

void can1_transmit_data(void)
{
	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 0x400;
	tx_message_struct.extended_id = 0;
	tx_message_struct.id_type = CAN_ID_STANDARD;
	tx_message_struct.frame_type = CAN_TFT_DATA;
	tx_message_struct.dlc = 8;
	tx_message_struct.data[0] = 0x11;
	tx_message_struct.data[1] = 0x22;
	tx_message_struct.data[2] = 0x33;
	tx_message_struct.data[3] = 0x44;
	tx_message_struct.data[4] = 0x55;
	tx_message_struct.data[5] = 0x66;
	tx_message_struct.data[6] = 0x77;
	tx_message_struct.data[7] = 0x88;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
	//while(can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)transmit_mailbox) != CAN_TX_STATUS_SUCCESSFUL);
}

void CAN1_RX0_IRQHandler(void)
{
	can_rx_message_type rx_message_struct;
	if(can_flag_get(CAN1,CAN_RF0MN_FLAG) != RESET)
	{
		gpio_bits_set(GPIOG,GPIO_PINS_4);
		can1_rx_tmr=0;
		can_message_receive(CAN1, CAN_RX_FIFO0, &rx_message_struct);
		check_can_rx_data(&rx_message_struct);
	}
}

void CAN1_SE_IRQHandler(void)
{
	__IO uint32_t err_index = 0;
	if(can_flag_get(CAN1,CAN_ETR_FLAG) != RESET)
	{
		err_index = CAN1->ests & 0x70;
		can_flag_clear(CAN1, CAN_ETR_FLAG);
		/* error type is stuff error */
		if(err_index == 0x00000010)
		{
		  /* when stuff error occur: in order to ensure communication normally,
		  user must restart can or send a frame of highest priority message here */
		}
	}
}


