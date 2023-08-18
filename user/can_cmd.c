/*
 * can_cmd.c
 *
 *  Created on: 2023 Aug 18
 *      Author: pelev
 */

#include "can_cmd.h"
#include "can_tx_stack.h"
#include "cluster_state.h"

tx_stack can_tx_stack;

extern uint8_t lcd_can_addr;
extern uint8_t cluster_addr;
extern uint8_t plc_can_addr;
extern uint16_t plc_clust_regs[CLUSTER_REGS_CNT];
extern uint16_t plc_net_regs[NET_REGS_CNT];

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

void send_heartbeat() {

	static uint8_t heartbeat_value = 1;
	tx_stack_data packet;

	packet.id = 0x0400 | 0x01 | (lcd_can_addr<<3) | (cluster_addr << 7);
	packet.length = 2;
	packet.data[0] = 0x49;
	packet.data[1] = heartbeat_value++;
	add_tx_can_packet(&can_tx_stack,&packet);
}

void send_packed_physical_digits(uint8_t start_bit, uint8_t state, uint8_t fault, uint8_t mask) {

	tx_stack_data packet;
	packet.id = 0x0400 | 0x07 | (lcd_can_addr<<3) | (cluster_addr << 7);	// event
	packet.length = 5;
	packet.data[0] = 0x01; // packed physical digits
	packet.data[1] = 0x01; // start bit
	packet.data[2] = state; // state
	packet.data[3] = fault; // fault
	packet.data[4] = mask; // mask
	add_tx_can_packet(&can_tx_stack,&packet);
}
