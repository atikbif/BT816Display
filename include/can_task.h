/*
 * can_task.h
 *
 *  Created on: 2023 Aug 18
 *      Author: pelev
 */

#ifndef INCLUDE_CAN_TASK_H_
#define INCLUDE_CAN_TASK_H_

#include <stdint.h>

void can_task_function(void *pvParameters);

void write_clust_bit(uint16_t num, uint8_t value);
void write_net_bit(uint16_t num, uint8_t value);
void write_clust_reg(uint16_t num, uint16_t value);
void write_net_reg(uint16_t num, uint16_t value);

#endif /* INCLUDE_CAN_TASK_H_ */
