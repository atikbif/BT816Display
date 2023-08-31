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
#include "message_archive.h"

#define MESSAGE_CNT		10

static uint8_t down_sign = 0;
static uint8_t up_sign = 0;

#define MAX_ALARM_MESSAGE_LENGTH	(64+1)
static uint8_t alarm_text[MAX_ALARM_MESSAGE_LENGTH];

static uint16_t first_visible_alarm_num = 0;
static uint16_t alarm_cnt = 0;

extern menu_list_t current_menu;

void init_alarm_info_menu() {
	first_visible_alarm_num = 0;
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


	bt816_cmd_setfont2(1,MEM_FONT22,0);

	alarm_cnt = get_archive_records_cnt();

	if(alarm_cnt==0) first_visible_alarm_num = 0;

	if(first_visible_alarm_num>=MESSAGE_CNT) up_sign=1;else up_sign=0;
	if(first_visible_alarm_num+MESSAGE_CNT<alarm_cnt) down_sign=1;else down_sign=0;

	if(first_visible_alarm_num>MAX_ARCHIVE_RECORDS_CNT-MESSAGE_CNT) {
		first_visible_alarm_num = MAX_ARCHIVE_RECORDS_CNT-MESSAGE_CNT;
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

	uint16_t step = 30;
	for(int i=0;i<MESSAGE_CNT;i++) {

		if(first_visible_alarm_num+i>=alarm_cnt) break;
		struct message_record rec;

		uint8_t rec_body[ARCHIVE_RECORD_LENGTH - ARCHIVE_RECORD_SYS_DATA_LENGTH];
		rec.ptr = rec_body;

		if(get_record_with_offset_from_last(first_visible_alarm_num+i, &rec)) {
			uint16_t length = get_archive_message(&rec, alarm_text, MAX_ALARM_MESSAGE_LENGTH);
			if(length) {

				if(rec.message_type==1) {
					bt816_cmd_dl(DL_COLOR_RGB | YELLOW);
					bt816_cmd_dl(DL_END);
				}else if(rec.message_type==2) {
					bt816_cmd_dl(DL_COLOR_RGB | RED);
					bt816_cmd_dl(DL_END);
				}

				bt816_cmd_text(60, 90+i*step, 1, 0, alarm_text);
				length = get_archive_message_time(&rec, alarm_text, MAX_ALARM_MESSAGE_LENGTH);
				bt816_cmd_text(400, 90+i*step, 1, 0, alarm_text);

				bt816_cmd_dl(DL_COLOR_RGB | WHITE);
				bt816_cmd_dl(DL_END);
			}
		}else break;

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
