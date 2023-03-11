/*
 * can.h
 *
 *  Created on: 2023 Mar 11
 *      Author: Roman
 */

#ifndef INCLUDE_CAN_H_
#define INCLUDE_CAN_H_

#include <stdint.h>

void can1_init(void);
void can1_transmit_data(void);
void send_heartbeat();
void write_clust_bit(uint16_t num, uint8_t value);
void write_net_bit(uint16_t num, uint8_t value);
void write_clust_reg(uint16_t num, uint16_t value);
void write_net_reg(uint16_t num, uint16_t value);

#endif /* INCLUDE_CAN_H_ */
