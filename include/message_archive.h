/*
 * message_archive.h
 *
 *  Created on: 2023 Aug 24
 *      Author: pelev
 */

#ifndef INCLUDE_MESSAGE_ARCHIVE_H_
#define INCLUDE_MESSAGE_ARCHIVE_H_

#include <stdint.h>

#define SECTOR_SIZE                      	4096
#define ARCHIVE_PAGE_SIZE					(SECTOR_SIZE)
#define ARCHIVE_RECORD_SYS_DATA_LENGTH		(10)
#define ARCHIVE_RECORD_LENGTH 				(16)
#define MAX_ARCHIVE_RECORDS_CNT				(ARCHIVE_PAGE_SIZE/ARCHIVE_RECORD_LENGTH)
#define ARCH_UPDATE_PERIOD					(30000)


enum {MSG_ARCH_BIT, MSG_ARCH_CHECK_FLASH, MSG_ARCH_CHECK_CAN};

struct message_record {
	uint8_t *ptr;			// record pointer
	uint32_t time;
	uint16_t message_id;	// bit sys
	uint8_t message_type;	// info warning alarm
	uint8_t length;
};

void init_archive();
void add_record_to_archive(struct message_record *rec);
uint16_t get_archive_records_cnt();
void clear_archive();
uint8_t get_record_with_offset_from_last(uint16_t offset, struct message_record *rec);
uint16_t get_archive_message(struct message_record *rec, uint8_t *out_buf, uint16_t max_length);
uint16_t get_archive_message_time(struct message_record *rec, uint8_t *out_buf, uint16_t max_length);

#endif /* INCLUDE_MESSAGE_ARCHIVE_H_ */
