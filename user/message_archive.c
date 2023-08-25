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

#define ARCH_HEADER_ID			(0x38A4)
#define ARCH_REC_ID				(0x39A5)


uint32_t archive_start_addr = 0x500000;

static uint8_t arch_buf[ARCHIVE_PAGE_SIZE];
static uint8_t arch_page_num = 0;
static uint8_t archive_new_record_flag = 0;

static uint16_t arch_rec_cnt = 0;
static uint16_t first_rec_pos = 0;

static uint32_t get_page_addr_by_rec_num(uint16_t rec_num) {
	uint32_t res = archive_start_addr + ARCHIVE_PAGE_SIZE;
	uint16_t real_rec_pos = (first_rec_pos + rec_num)%MAX_ARCHIVE_RECORDS_CNT;
	uint32_t addition = (uint32_t)ARCHIVE_RECORD_LENGTH*(real_rec_pos);
	addition -= (addition%ARCHIVE_PAGE_SIZE);
	res += addition;
	return res;
}

static uint32_t archive_get_current_page_address() {
	uint32_t res = archive_start_addr + ARCHIVE_PAGE_SIZE;
	if(arch_rec_cnt>MAX_ARCHIVE_RECORDS_CNT) arch_rec_cnt = 0;
	if(first_rec_pos>=MAX_ARCHIVE_RECORDS_CNT) first_rec_pos = 0;
	uint16_t last_rec_pos = (first_rec_pos + (arch_rec_cnt?(arch_rec_cnt-1):arch_rec_cnt))%MAX_ARCHIVE_RECORDS_CNT;
	uint32_t addition = (uint32_t)ARCHIVE_RECORD_LENGTH*(last_rec_pos);
	addition -= (addition%ARCHIVE_PAGE_SIZE);
	res += addition;
	return res;
}

void init_archive() {
	uint8_t check_flag = 1;
	bt816_cmd_flashread(0, archive_start_addr, ARCHIVE_PAGE_SIZE);
	vTaskDelay(1);
	for(uint16_t i=0;i<ARCHIVE_HEADER_LENGTH;i++) {
		arch_buf[i] = bt816_mem_read8(i);
	}
	if ((arch_buf[0]==(ARCH_HEADER_ID>>8)&0xFF)&&(arch_buf[1]==(ARCH_HEADER_ID&0xFF))) {
		if(GetCRC16(arch_buf, ARCHIVE_HEADER_LENGTH)) check_flag = 0;
		else {
			arch_rec_cnt = arch_buf[2];
			arch_rec_cnt = arch_rec_cnt << 8;
			arch_rec_cnt |= arch_buf[3];
			first_rec_pos = arch_buf[4];
			first_rec_pos = first_rec_pos << 8;
			first_rec_pos |= arch_buf[5];
		}
	}else check_flag = 0;
	if(check_flag==0) {
		clear_archive();
	}else {
		for(uint16_t i=0;i<ARCHIVE_PAGE_SIZE;i++) arch_buf[i] = 0;
		if(arch_rec_cnt){
			uint32_t addr = archive_get_current_page_address();
			bt816_cmd_flashread(0, addr, ARCHIVE_PAGE_SIZE);
			for(uint16_t i=0;i<ARCHIVE_PAGE_SIZE;i++) {
				arch_buf[i] = bt816_mem_read8(i);
			}
		}
	}
}

void add_record_to_archive(struct message_record *rec) {
	uint32_t prev_addr = archive_get_current_page_address();
	arch_rec_cnt++;
	if(arch_rec_cnt>MAX_ARCHIVE_RECORDS_CNT) {
		arch_rec_cnt = MAX_ARCHIVE_RECORDS_CNT;
		first_rec_pos++;
		if(first_rec_pos>=MAX_ARCHIVE_RECORDS_CNT) first_rec_pos = 0;
	}
	uint32_t cur_addr = archive_get_current_page_address();
	if(cur_addr!=prev_addr) {
		bt816_cmd_memwrite(0,ARCHIVE_PAGE_SIZE,arch_buf);
		vTaskDelay(1);
		bt816_cmd_flashupdate(prev_addr,0,ARCHIVE_PAGE_SIZE);
	}
	uint16_t offset = ((first_rec_pos + (arch_rec_cnt-1))%MAX_ARCHIVE_RECORDS_CNT)*ARCHIVE_RECORD_LENGTH;

	if(rec->length>ARCHIVE_RECORD_LENGTH-ARCHIVE_RECORD_SYS_DATA_LENGTH) {
		rec->length = ARCHIVE_RECORD_LENGTH-ARCHIVE_RECORD_SYS_DATA_LENGTH;
	}

	arch_buf[offset] = (ARCH_REC_ID>>8)&0xFF;
	arch_buf[offset+1] = ARCH_REC_ID&0xFF;
	arch_buf[offset+2] = (rec->time>>24)&0xFF;
	arch_buf[offset+3] = (rec->time>>16)&0xFF;
	arch_buf[offset+4] = (rec->time>>8)&0xFF;
	arch_buf[offset+5] = (rec->time)&0xFF;
	arch_buf[offset+6] = (rec->message_id>>8)&0xFF;
	arch_buf[offset+7] = rec->message_id&0xFF;
	arch_buf[offset+8] = rec->message_type;
	arch_buf[offset+9] = (rec->length>>8)&0xFF;
	arch_buf[offset+10] = (rec->length&0xFF);
	arch_buf[offset+11] = 0; // crch place
	arch_buf[offset+12] = 0; // crcl place

	// add zero tail
	uint16_t i = 13;
	while(i<ARCHIVE_RECORD_SYS_DATA_LENGTH) {
		arch_buf[offset+i] = 0;
		i++;
	}

	for(uint16_t i=0;i<ARCHIVE_RECORD_LENGTH-ARCHIVE_RECORD_SYS_DATA_LENGTH;i++) {
		if(i<rec->length) arch_buf[offset+ARCHIVE_RECORD_SYS_DATA_LENGTH+i] = rec->ptr[i];
		else arch_buf[offset+ARCHIVE_RECORD_SYS_DATA_LENGTH+i] = 0;	// zero tail
	}

	uint16_t crc = GetCRC16(&arch_buf[offset], ARCHIVE_RECORD_SYS_DATA_LENGTH + rec->length);
	arch_buf[offset+11] = crc >> 8; 	// crch place
	arch_buf[offset+12] = crc & 0xFF; 	// crcl place

	archive_new_record_flag = 1;
}

uint16_t get_archive_records_cnt() {
	return arch_rec_cnt;
}

void clear_archive() {
	for(uint16_t i=0;i<ARCHIVE_HEADER_LENGTH;i++) arch_buf[i] = 0;
	arch_buf[0] = (ARCH_HEADER_ID>>8)&0xFF;
	arch_buf[1] = ARCH_HEADER_ID&0xFF;
	arch_rec_cnt = 0;
	first_rec_pos = 0;
	bt816_cmd_memwrite(0,ARCHIVE_HEADER_LENGTH,arch_buf);
	vTaskDelay(1);
	bt816_cmd_flashupdate(archive_start_addr,0,4096);
	arch_page_num = 0;
	archive_new_record_flag = 0;
	for(uint16_t i=0;i<ARCHIVE_PAGE_SIZE;i++) arch_buf[i] = 0;
}

uint8_t get_record_from_archive(uint16_t rec_num, struct message_record *rec) {
	uint8_t rec_buf[ARCHIVE_RECORD_LENGTH];
	uint32_t addr = get_page_addr_by_rec_num(rec_num);
	bt816_cmd_flashread(0, addr, ARCHIVE_PAGE_SIZE);
	vTaskDelay(1);
	uint16_t offset = 0;
	for(uint16_t i=0;i<ARCHIVE_RECORD_SYS_DATA_LENGTH;i++) {
		rec_buf[i] = bt816_mem_read8(offset + i);
	}

	if(rec_buf[0]!=(ARCH_REC_ID>>8)&0xFF) return 0;
	if(rec_buf[1]!=ARCH_REC_ID&0xFF) return 0;
	uint16_t length = rec_buf[9];
	length = length<<8;
	length |= rec_buf[10];
	if(length>ARCHIVE_RECORD_LENGTH-ARCHIVE_RECORD_SYS_DATA_LENGTH) return 0;
	uint16_t crc = rec_buf[11];
	crc = crc << 8;
	crc |= rec_buf[12];
	rec_buf[11] = 0;
	rec_buf[12] = 0;
	if(GetCRC16(rec_buf, ARCHIVE_RECORD_SYS_DATA_LENGTH+length)!=crc) return 0;
	rec->length = length;
	rec->time = rec_buf[2];
	rec->time = rec->time<<8;
	rec->time |= rec_buf[3];
	rec->time = rec->time<<8;
	rec->time |= rec_buf[4];
	rec->time = rec->time<<8;
	rec->time |= rec_buf[5];
	rec->message_id = rec_buf[6];
	rec->message_id = rec->message_id<<8;
	rec->message_id |= rec_buf[7];
	rec->message_type = rec_buf[8];
	for(uint16_t i=0;i<length;i++) rec->ptr[i] = rec_buf[ARCHIVE_RECORD_SYS_DATA_LENGTH+i];
	return 1;
}

void check_new_records_update() {
	static uint16_t archive_update_tmr = 0;
	archive_update_tmr++;
	if(archive_update_tmr>=ARCH_UPDATE_PERIOD) {
		archive_update_tmr = 0;
		if(archive_new_record_flag) {
			bt816_cmd_memwrite(0,ARCHIVE_PAGE_SIZE,arch_buf);
			vTaskDelay(1);
			uint32_t addr = archive_get_current_page_address();
			bt816_cmd_flashupdate(addr,0,ARCHIVE_PAGE_SIZE);
			archive_new_record_flag = 0;
		}
	}
}

