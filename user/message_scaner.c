/*
 * message_scaner.c
 *
 *  Created on: 2023 Aug 23
 *      Author: pelev
 */

#include "message_scaner.h"
#include "cluster_state.h"
#include "config.h"
#include "message_archive.h"
#include "cur_time.h"
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
			if((msg_conf[i].value!=msg_conf[i].prev_value) && msg_conf[i].value) {
				// add message to archive
				struct message_record rec;
				rec.message_type = msg_conf[i].message_type;
				rec.message_id = MSG_ARCH_BIT;
				rec.time = time_to_uint32();
				uint8_t rec_body[16];
				rec_body[0] = 1; // version
				rec_body[1] = msg_conf[i].var_type;
				rec_body[2] = (msg_conf[i].var_index>>8)&0xFF;
				rec_body[3] = msg_conf[i].var_index&0xFF;
				rec_body[4] = msg_conf[i].value;
				rec.length = 5;
				rec.ptr = rec_body;
				add_record_to_archive(&rec);
			}
			msg_conf[i].prev_value = msg_conf[i].value;
		}
	}
}

uint16_t get_active_messages_cnt() {
	uint16_t res = 0;

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
			if(msg_conf[i].value) {
				res++;
			}
		}
	}
	return res;
}

uint8_t get_message_by_number(uint8_t num, struct bit_message_conf *message, uint8_t *name_ptr) {
	uint8_t pos = 0;

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
			if(msg_conf[i].value) {
				if(num==pos) {
					*message = msg_conf[i];
					get_message_name(i, name_ptr);
					return 1;
				}
				pos++;
			}
		}
	}
	return 0;
}
