/*
 * alarm_list_menu.c
 *
 *  Created on: 2023 May 11
 *      Author: pelev
 */

#include "alarm_list_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "string.h"

#define MESSAGE_CNT		16

const char check_flash_ok[] = "\x46\x4c\x41\x53\x48\x20\xd0\x9f\xd0\xa0\xd0\x9e\xd0\x92\xd0\x95\xd0\xa0\xd0\x95\xd0\x9d\xd0\x9e";
const char check_flash_err[] = "\x46\x4c\x41\x53\x48\x20\xd0\x9e\xd0\xa8\xd0\x98\xd0\x91\xd0\x9a\xd0\x90";

static uint8_t down_sign = 0;
static uint8_t up_sign = 0;

#define MAX_ALARM_MESSAGE_LENGTH	(64+1)
static uint8_t alarm_text[MAX_ALARM_MESSAGE_LENGTH];

alarm_info alarm_list[ALARM_LIST_SIZE];

static uint16_t first_visible_alarm_num = 0;
static uint16_t alarm_cnt = 0;

extern menu_list_t current_menu;

static uint8_t get_additional_alarm_info(uint32_t addr, uint8_t *buf);
static uint8_t get_alarm_description(uint16_t index);
static uint8_t print_date(uint8_t *buf, time_info time);

void clear_alarm_list() {
	for(uint16_t i=0;i<ALARM_LIST_SIZE;i++) alarm_list[i].alarm_id=0;
	alarm_cnt = 0;
}

void add_alarm(alarm_info info) {
	for(uint16_t i=0;i<ALARM_LIST_SIZE-1;i++) alarm_list[ALARM_LIST_SIZE-1-i]=alarm_list[ALARM_LIST_SIZE-2-i];
	if(alarm_cnt<ALARM_LIST_SIZE) alarm_cnt++;
	alarm_list[0] = info;
}

uint8_t get_additional_alarm_info(uint32_t addr, uint8_t *buf) {
	return 0;
}

uint8_t print_date(uint8_t *buf, time_info time) {
	buf[0]=time.hour/10+'0';
	buf[1]=time.hour%10+'0';
	buf[2]=':';
	buf[3]=time.min/10+'0';
	buf[4]=time.min%10+'0';
	buf[5]=':';
	buf[6]=time.sec/10+'0';
	buf[7]=time.sec%10+'0';
	buf[8]=' ';
	buf[9]=time.date/10+'0';
	buf[10]=time.date%10+'0';
	buf[11]='/';
	buf[12]=time.month/10+'0';
	buf[13]=time.month%10+'0';
	buf[14]='/';
	buf[15]=time.year/10+'0';
	buf[16]=time.month%10+'0';
	return 17;
}

uint8_t get_alarm_description(uint16_t index) {
	for(uint8_t i=0;i<MAX_ALARM_MESSAGE_LENGTH;i++) alarm_text[i]=0;

	if(alarm_list[index].alarm_id) {
		uint16_t j = 0;
		for(j=0;j<MAX_ALARM_MESSAGE_LENGTH-1;j++) alarm_text[j]=0;
		uint16_t pos = 0;
		uint16_t value = index + 1;
		while(value>=1000) value-=1000;
		if(value>=100) alarm_text[pos++] = value/100 + '0';
		while(value>=100) value-=100;
		if(value>=10) alarm_text[pos++] = value/10 + '0';
		while(value>=10) value-=10;
		alarm_text[pos++] = value + '0';
		while(pos<4) alarm_text[pos++]=' ';
		uint16_t length = 0;
		switch(alarm_list[index].alarm_id) {
			case CHECK_EXT_FLASH_OK:
				length = strlen(check_flash_ok);
				for(j=0;j<length;j++) alarm_text[pos++] = check_flash_ok[j];
				break;
			case CHECK_EXT_FLASH_ERR:
				length = strlen(check_flash_err);
				for(j=0;j<length;j++) alarm_text[pos++] = check_flash_err[j];
				break;
		}
		while(pos<40) alarm_text[pos++]=' ';
		pos+=print_date(&alarm_text[pos],alarm_list[index].time);
		return pos+1;
	}
	return 0;
}

void alarm_info_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x130000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);


	bt816_cmd_setfont2(1,MEM_FONT14,0);

	if(first_visible_alarm_num>=MESSAGE_CNT) up_sign=1;else up_sign=0;
	if(first_visible_alarm_num+MESSAGE_CNT<alarm_cnt) down_sign=1;else down_sign=0;

	if(first_visible_alarm_num>ALARM_LIST_SIZE-MESSAGE_CNT) {
		first_visible_alarm_num = ALARM_LIST_SIZE-MESSAGE_CNT;
	}

	if(up_sign) {
		bt816_cmd_dl(DL_COLOR_RGB | BLUE);
		bt816_cmd_dl(DL_END);
		bt816_cmd_dl(DL_BEGIN | BT816_LINE_STRIP);
		bt816_cmd_dl(VERTEX2F(760*16, 49*16));
		bt816_cmd_dl(VERTEX2F(775*16, 34*16));
		bt816_cmd_dl(VERTEX2F(790*16, 49*16));
		bt816_cmd_dl(VERTEX2F(760*16, 49*16));
		bt816_cmd_dl(DL_END);
	}

	if(down_sign) {
		bt816_cmd_dl(DL_COLOR_RGB | BLUE);
			bt816_cmd_dl(DL_END);
			bt816_cmd_dl(DL_BEGIN | BT816_LINE_STRIP);
			bt816_cmd_dl(VERTEX2F(760*16, 54*16));
			bt816_cmd_dl(VERTEX2F(775*16, 69*16));
			bt816_cmd_dl(VERTEX2F(790*16, 54*16));
			bt816_cmd_dl(VERTEX2F(760*16, 54*16));
			bt816_cmd_dl(DL_END);
	}

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	uint16_t step = 20;
	for(int i=0;i<MESSAGE_CNT;i++) {
		uint16_t length = get_alarm_description(i+first_visible_alarm_num);
		if(length) {
			bt816_cmd_text(60, 90+i*step, 1, 0, alarm_text);
		}
	}


	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_LEFT:
			current_menu = MENU_MAIN;
			break;
		case KEY_RIGHT:
			current_menu = MENU_CLR_ALARMS;
			break;
		case KEY_UP:
			if(first_visible_alarm_num>=MESSAGE_CNT) first_visible_alarm_num-=MESSAGE_CNT;
			else first_visible_alarm_num = 0;
			break;
		case KEY_DOWN:
			if(first_visible_alarm_num+MESSAGE_CNT<alarm_cnt) first_visible_alarm_num+=MESSAGE_CNT;
			break;
	}
}
