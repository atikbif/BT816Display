/*
 * message_scaner.c
 *
 *  Created on: 2023 Aug 23
 *      Author: pelev
 */

#include "message_scaner.h"
#include "cluster_state.h"
#include "config.h"

struct bit_message_conf msg_conf[MAX_MSG_CNT];

extern cluster cl;

void scan_messages() {
	for(uint16_t i=0;i<MAX_MSG_CNT;i++) {
		if(msg_conf[i].used) {
			switch(msg_conf[i].var_type) {
				case 0:
					msg_conf[i].value = cl.cluster_bits[msg_conf[i].var_index];
					break;
				case 2:
					msg_conf[i].value = cl.net_bits[msg_conf[i].var_index];
					break;
			}
			if(msg_conf[i].value!=msg_conf[i].prev_value) {
				// add message to archive
			}
			msg_conf[i].prev_value = msg_conf[i].value;
		}
	}
}
