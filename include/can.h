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
void can_write_from_stack();

#endif /* INCLUDE_CAN_H_ */
