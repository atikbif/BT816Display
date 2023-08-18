/*
 * can_task.c
 *
 *  Created on: 2023 Aug 18
 *      Author: pelev
 */

#include "can_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "can.h"
#include "can_tx_stack.h"
#include "at32f435_437_board.h"
#include "cluster_state.h"

#define DI_CNT		8

tx_stack can_tx_stack;
uint16_t cur_di = 0;
uint16_t prev_di = 0;
uint16_t update_di = 0;

extern uint8_t lcd_can_addr;
extern uint8_t cluster_addr;
extern uint8_t plc_can_addr;
extern uint16_t plc_clust_regs[CLUSTER_REGS_CNT];
extern uint16_t plc_net_regs[NET_REGS_CNT];

static void update_di_data() {
	static uint16_t di_tmr=0;
	uint8_t di_mask = 0x00;
	uint8_t send_fault = 0x00;
	uint8_t send_state = 0x00;
	uint16_t di_fitted = 0x00FF;
	uint8_t i = 0;

	tx_stack_data packet;

	uint16_t cur_di = get_di_state();
	for(int i=0;i<DI_CNT;i++) {
		if((prev_di&(1<<i))!=(cur_di&(1<<i))) {
			update_di |= 1<<i;
		}
	}

	di_tmr++;
	if(di_tmr==20) {
		di_mask = update_di & di_fitted & 0xFF;
		if(di_mask)  {
			for(i=0;i<8;i++) {
				if(di_mask & (1<<i)) {
					if(cur_di & (1<<i)) send_state |= 1<<i;
				}
			}
			packet.id = 0x0400 | 0x07 | (lcd_can_addr<<3) | (cluster_addr << 7);	// event
			packet.length = 5;
			packet.data[0] = 0x01; // packed physical digits
			packet.data[1] = 0x01; // start bit
			packet.data[2] = send_state; // state
			packet.data[3] = send_fault; // fault
			packet.data[4] = di_mask; // mask
			add_tx_can_packet(&can_tx_stack,&packet);
			update_di &= 0xFF00;
			send_state = 0;
			send_fault = 0;
			di_tmr=0;
		}
	}
	prev_di = cur_di;
}

void write_clust_bit(uint16_t num, uint8_t value) {
	tx_stack_data packet;

	packet.id = 0x0400 | 0x07 | (plc_can_addr<<3) | (cluster_addr << 7);
	packet.length = 4;
	packet.data[0] = 0x03;
	packet.data[1] = num+16;
	if(value) packet.data[2] = 0x01;
	else packet.data[2] = 0x00;
	packet.data[3] = 0x01;
	add_tx_can_packet(&can_tx_stack,&packet);
}

void write_net_bit(uint16_t num, uint8_t value) {

	tx_stack_data packet;

	packet.id = 0x0400 | 0x07 | (lcd_can_addr<<3) | (cluster_addr << 7);
	packet.length = 4;
	packet.data[0] = 0x0E;
	packet.data[1] = cluster_addr*16+num+1;
	if(value) packet.data[2] = 0x01;
	else packet.data[2] = 0x00;
	packet.data[3] = 0x01;
	add_tx_can_packet(&can_tx_stack,&packet);
}

void write_clust_reg(uint16_t num, uint16_t value) {

	tx_stack_data packet;

	packet.id = 0x0400 | 0x07 | (lcd_can_addr<<3) | (cluster_addr << 7);
	packet.length = 4;
	packet.data[0] = 0x06;
	packet.data[1] = num+17;
	packet.data[2] = plc_clust_regs[num]&0xFF;
	packet.data[3] = plc_clust_regs[num]>>8;
	add_tx_can_packet(&can_tx_stack,&packet);
}

void write_net_reg(uint16_t num, uint16_t value) {

	tx_stack_data packet;

	packet.id = 0x0400 | 0x07 | (lcd_can_addr<<3) | (cluster_addr << 7);
	packet.length = 4;
	packet.data[0] = 0x0F;
	packet.data[1] = cluster_addr*16+num+1;
	packet.data[2] = plc_net_regs[num]&0xFF;
	packet.data[3] = plc_net_regs[num]>>8;
	add_tx_can_packet(&can_tx_stack,&packet);
}

static void send_heartbeat() {

	static uint8_t heartbeat_value = 1;
	tx_stack_data packet;

	packet.id = 0x0400 | 0x01 | (lcd_can_addr<<3) | (cluster_addr << 7);
	packet.length = 2;
	packet.data[0] = 0x49;
	packet.data[1] = heartbeat_value++;
	add_tx_can_packet(&can_tx_stack,&packet);
}

void can_task_function(void *pvParameters) {
	uint16_t heartbeat_tmr = 0;
	init_can_tx_stack(&can_tx_stack);
	while(1)
	{
		heartbeat_tmr++;
		if(heartbeat_tmr>=900) {
			heartbeat_tmr = 0;
			send_heartbeat();
		}
		update_di_data();
		can_write_from_stack();
		vTaskDelay(1);
	}
}
