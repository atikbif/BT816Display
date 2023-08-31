/*
 * message_archive.c
 *
 *  Created on: 2023 Aug 24
 *      Author: pelev
 */

#include "message_archive.h"
#include "crc.h"
#include "bt816_cmd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
#include "string.h"

#define JD0               2451911

static uint8_t arch_buf[ARCHIVE_PAGE_SIZE];

static uint8_t archive_new_record_flag = 0;

static uint8_t ext_flash_used = 0;
static uint8_t *first_rec_ptr = 0;
static uint8_t *last_rec_ptr = 0;

static const char test_flash_ok[] = "\xd0\xa2\xd0\xb5\xd1\x81\xd1\x82\x20\x46\x6c\x61\x73\x68\x20\x2d\x20\xd1\x83\xd1\x81\xd0\xbf\xd0\xb5\xd1\x88\xd0\xbd\xd0\xbe";
static const char test_flash_err[] = "\xd0\xa2\xd0\xb5\xd1\x81\xd1\x82\x20\x46\x6c\x61\x73\x68\x20\xd0\xbe\xd1\x88\xd0\xb8\xd0\xb1\xd0\xba\xd0\xb0";
static const char can_link_err[] = "\xd0\x9f\xd0\x9b\xd0\x9a\x20\x43\x41\x4e\x20\xd0\xbe\xd1\x88\xd0\xb8\xd0\xb1\xd0\xba\xd0\xb0";

static void save_archive() {

}

static void open_archive() {

}

static uint8_t check_record(uint8_t *ptr) {
	uint8_t buf[ARCHIVE_RECORD_LENGTH];
	uint8_t res = 0;
	memcpy(buf,ptr,ARCHIVE_RECORD_LENGTH);
	uint8_t length = buf[7];
	uint8_t crc_h = buf[8];
	uint8_t crc_l = buf[9];
	buf[8] = 0;
	buf[9] = 0;
	uint16_t crc = GetCRC16(buf, ARCHIVE_RECORD_SYS_DATA_LENGTH + length);
	if((((crc>>8)&0xFF)==crc_h) && ((crc&0xFF)==crc_l)) res = 1;
	return res;
}

void init_archive() {
	ext_flash_used = 0;
	clear_archive();
	if(ext_flash_used) {
		open_archive();
	}
}

void add_record_to_archive(struct message_record *rec) {
	uint8_t *ptr = 0;

	if(last_rec_ptr==0) {
		last_rec_ptr= &arch_buf[0];
		ptr = last_rec_ptr;
	}else {
		ptr=last_rec_ptr+ARCHIVE_RECORD_LENGTH;
		if(ptr>=&arch_buf[ARCHIVE_PAGE_SIZE-1]) {
			if(ext_flash_used) save_archive();
			ptr = &arch_buf[0];
		}
	}

	if(rec->length > (ARCHIVE_RECORD_LENGTH - ARCHIVE_RECORD_SYS_DATA_LENGTH)) {
		rec->length = ARCHIVE_RECORD_LENGTH - ARCHIVE_RECORD_SYS_DATA_LENGTH;
	}

	ptr[0] = (rec->time>>24)&0xFF;
	ptr[1] = (rec->time>>16)&0xFF;
	ptr[2] = (rec->time>>8)&0xFF;
	ptr[3] = (rec->time)&0xFF;
	ptr[4] = (rec->message_id>>8)&0xFF;
	ptr[5] = rec->message_id&0xFF;
	ptr[6] = rec->message_type;
	ptr[7] = rec->length;
	ptr[8] = 0; // crch place
	ptr[9] = 0; // crcl place

	for(uint16_t i=0;i<ARCHIVE_RECORD_LENGTH-ARCHIVE_RECORD_SYS_DATA_LENGTH;i++) {
		if(i<rec->length) ptr[ARCHIVE_RECORD_SYS_DATA_LENGTH+i] = rec->ptr[i];
		else ptr[ARCHIVE_RECORD_SYS_DATA_LENGTH+i] = 0;	// zero tail
	}

	uint16_t crc = GetCRC16(ptr, ARCHIVE_RECORD_SYS_DATA_LENGTH + rec->length);
	ptr[8] = crc >> 8; 	// crch place
	ptr[9] = crc & 0xFF; 	// crcl place

	last_rec_ptr = ptr;

	get_archive_records_cnt();

	archive_new_record_flag = 1;
}

uint16_t get_archive_records_cnt() {
	uint16_t res = 0;
	uint8_t *ptr = last_rec_ptr;
	if(ptr) {
		while(check_record(ptr)) {
			res++;
			if(res>=MAX_ARCHIVE_RECORDS_CNT) break;
			if(ptr>&arch_buf[0]) {
				ptr-=ARCHIVE_RECORD_LENGTH;
			}else {
				ptr = &arch_buf[ARCHIVE_PAGE_SIZE-ARCHIVE_RECORD_LENGTH];
			}
		}
	}
	return res;
}

uint8_t get_record_with_offset_from_last(uint16_t offset, struct message_record *rec) {
	uint8_t *ptr = 0;
	uint8_t res = 0;
	if(last_rec_ptr) {
		ptr = last_rec_ptr;
		while(offset) {
			if(ptr>&arch_buf[0]) {
				ptr-=ARCHIVE_RECORD_LENGTH;
			}else {
				ptr = &arch_buf[ARCHIVE_PAGE_SIZE-ARCHIVE_RECORD_LENGTH];
			}
			offset--;
		}
	}
	if(ptr) {
		uint8_t length = ptr[7];
		if(length>ARCHIVE_RECORD_LENGTH-ARCHIVE_RECORD_SYS_DATA_LENGTH) return 0;
		if(check_record(ptr)==0) return 0;
		rec->length = length;
		rec->time = ptr[0];
		rec->time = rec->time<<8;
		rec->time |= ptr[1];
		rec->time = rec->time<<8;
		rec->time |= ptr[2];
		rec->time = rec->time<<8;
		rec->time |= ptr[3];
		rec->message_id = ptr[4];
		rec->message_id = rec->message_id<<8;
		rec->message_id |= ptr[5];
		rec->message_type = ptr[6];
		for(uint16_t i=0;i<length;i++) rec->ptr[i] = ptr[ARCHIVE_RECORD_SYS_DATA_LENGTH+i];
		res = 1;
	}
	return res;
}

void clear_archive() {
	archive_new_record_flag = 0;
	first_rec_ptr = 0;
	last_rec_ptr = 0;
	for(uint16_t i=0;i<ARCHIVE_PAGE_SIZE;i++) arch_buf[i] = 0;
}

void check_new_records_update(uint16_t step) {
	static uint16_t archive_update_tmr = 0;
	if(ext_flash_used) {
		archive_update_tmr+=step;
		if(archive_update_tmr>=ARCH_UPDATE_PERIOD) {
			archive_update_tmr = 0;
			if(archive_new_record_flag) {
				 save_archive();
				archive_new_record_flag = 0;
			}
		}
	}
}

uint16_t get_archive_message(struct message_record *rec, uint8_t *out_buf, uint16_t max_length) {
	uint16_t res = 0;
	uint16_t index = 0;
	uint16_t i = 0;
	for(i=0;i<max_length;i++) out_buf[i]=0;
	switch(rec->message_id) {
		case MSG_ARCH_CHECK_FLASH:
			if(rec->ptr[0]) {
				for(i=0;i<sizeof(test_flash_ok);i++) {
					if(max_length && (i<(max_length-1))) {
						out_buf[i] = test_flash_ok[i];
						res++;
					}
				}
			}else {
				for(i=0;i<sizeof(test_flash_err);i++) {
					if(max_length && (i<(max_length-1))) {
						out_buf[i] = test_flash_err[i];
						res++;
					}
				}
			}
			break;
		case MSG_ARCH_CHECK_CAN:
			if(rec->ptr[0]==0) {
				for(i=0;i<sizeof(can_link_err);i++) {
					if(max_length && (i<(max_length-1))) {
						out_buf[i] = can_link_err[i];
						res++;
					}
				}
			}
			break;
		case MSG_ARCH_BIT:
			if(rec->ptr[0]==1) {	// version
				uint8_t bit_type = rec->ptr[1];
				index = rec->ptr[2];
				index = index << 8;
				index |= rec->ptr[3];

				if(rec->ptr[4]) {		// value
					uint8_t bit_type = rec->ptr[1];
					if(bit_type==0) {	// cluster bit
						res = get_cluster_bit_name(index,out_buf,max_length);
					}else if(bit_type==2) {	// net bit
						res = get_net_bit_name(index,out_buf,max_length);
					}
				}
			}
			break;
	}
	if(max_length && (res>=max_length)) {
		out_buf[max_length-1] = 0;
	}
	return res;
}

uint16_t get_archive_message_time(struct message_record *rec, uint8_t *out_buf, uint16_t max_length) {
	uint16_t i = 0;
	uint16_t res =0;
	for(i=0;i<max_length;i++) out_buf[i]=0;
	uint32_t ace;
	uint8_t b,d,m;

	ace = (rec->time/86400) + 32044 + JD0;
	b = (4*ace + 3)/146097;
	ace = ace - ((146097*b)/4);
	d = (4*ace + 3)/1461;
	ace = ace - ((1461*d)/4);
	m = (5*ace + 2)/153;
	uint8_t date = ace - ((153*m+2)/5) + 1;
	uint8_t month = m + 3 - (12*(m/10));
	uint8_t year = 100*b + d-4800 + (m/10) - 2000;
	uint8_t hour = (rec->time/3600)%24;
	uint8_t min = (rec->time/60)%60;
	uint8_t sec = (rec->time%60);

	out_buf[res++]=hour/10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=hour%10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=':';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=min/10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=min%10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=':';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=sec/10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=sec%10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=' ';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=date/10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=date%10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]='/';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=month/10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=month%10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]='/';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=year/10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=year%10+'0';
	if(res>=max_length) {
		out_buf[max_length-1]=0;
		return max_length-1;
	}
	out_buf[res++]=0;
	return res;
}

