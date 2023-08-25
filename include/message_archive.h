/*
 * message_archive.h
 *
 *  Created on: 2023 Aug 24
 *      Author: pelev
 */

#ifndef INCLUDE_MESSAGE_ARCHIVE_H_
#define INCLUDE_MESSAGE_ARCHIVE_H_

#include <stdint.h>

#define ARCHIVE_PAGE_SIZE					(4096)
#define ARCHIVE_HEADER_LENGTH				(64)
#define MAX_ARCHIVE_RECORDS_CNT				(128)
#define ARCHIVE_RECORD_SYS_DATA_LENGTH		(32)
#define ARCHIVE_RECORD_LENGTH 				(128)
#define ARCH_UPDATE_PERIOD					(3000)
#define ARCHIVE_REC_CNT_IN_A_PAGE			(ARCHIVE_PAGE_SIZE/ARCHIVE_RECORD_LENGTH)

enum {MSG_ARCH_BIT, MSG_ARCH_CHECK_FLASH, MSG_ARCH_CHECK_CAN};

struct message_record {
	uint32_t time;
	uint8_t *ptr;			// record pointer
	uint16_t message_id;	// bit sys
	uint16_t length;
	uint8_t message_type;	// info warning alarm

};

void init_archive();
void add_record_to_archive(struct message_record *rec);
uint16_t get_archive_records_cnt();
void clear_archive();
uint8_t get_record_from_archive(uint16_t rec_num, struct message_record *rec);
void check_new_records_update(uint16_t step);

#endif /* INCLUDE_MESSAGE_ARCHIVE_H_ */
