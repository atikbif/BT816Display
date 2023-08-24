/*
 * message_scaner.h
 *
 *  Created on: 2023 Aug 23
 *      Author: pelev
 */

#ifndef INCLUDE_MESSAGE_SCANER_H_
#define INCLUDE_MESSAGE_SCANER_H_

#include <stdint.h>

#define MAX_MSG_CNT		63

struct bit_message_conf{
	uint8_t var_type;
	uint8_t message_type;
	uint16_t var_index;
	uint8_t value;
	uint8_t prev_value;
	uint8_t used;
};

void scan_messages();


#endif /* INCLUDE_MESSAGE_SCANER_H_ */
