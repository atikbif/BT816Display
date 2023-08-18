/*
 * can_commands.h
 *
 *  Created on: 2023 Aug 18
 *      Author: pelev
 */

#ifndef INCLUDE_CAN_CMD_H_
#define INCLUDE_CAN_CMD_H_

#include <stdint.h>

void write_clust_bit(uint16_t num, uint8_t value);
void write_net_bit(uint16_t num, uint8_t value);
void write_clust_reg(uint16_t num, uint16_t value);
void write_net_reg(uint16_t num, uint16_t value);
void send_heartbeat();
void send_packed_physical_digits(uint8_t start_bit, uint8_t state, uint8_t fault, uint8_t mask);

#endif /* INCLUDE_CAN_CMD_H_ */
