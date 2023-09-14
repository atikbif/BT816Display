/*
 * state_menu.c
 *
 *  Created on: 2023 Aug 18
 *      Author: pelev
 */

#include "state_menu.h"

#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "cluster_state.h"
#include "config.h"
#include "demo_design_menu.h"
#include "message_scaner.h"
#include "config.h"
#include "time.h"
#include "cur_time.h"
#include "at32f435_437_ertc.h"

#include "message_scaner.h"
#include "prog_menu.h"
#include "appl_info_menu.h"

extern menu_list_t current_menu;
extern ertc_time_type dev_time;
extern uint8_t plc_can_link;
extern uint8_t prog_mode_flag;

extern cluster cl;
extern appl_info_data_type appl_info_data;

static uint8_t down_sign = 0;
static uint8_t up_sign = 0;

#define MAX_MESSAGE_LENGTH	(64+1)
#define SCREEN_MSG_CNT		(10)

static uint8_t message_text[MAX_MESSAGE_LENGTH];
static uint8_t msg_cnt = 0;

static uint16_t first_visible_message_num = 0;

static uint16_t prev_key = 0;

static uint8_t print_date(uint8_t *buf, time_info time) {
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
	buf[16]=time.year%10+'0';
	buf[17]=0;
	return 18;
}

void init_state_menu() {
	first_visible_message_num = 0;
}

void state_menu(uint16_t key) {

	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t mem_addr = 0x120000;

	bt816_cmd_setbitmap(0x800000 | (mem_addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(1,MEM_FONT22,0);

	msg_cnt = get_active_messages_cnt();

	if(first_visible_message_num>=SCREEN_MSG_CNT) up_sign=1;else up_sign=0;
	if(first_visible_message_num+SCREEN_MSG_CNT<msg_cnt) down_sign=1;else down_sign=0;

	if(first_visible_message_num>MAX_MSG_CNT-SCREEN_MSG_CNT) {
		first_visible_message_num = MAX_MSG_CNT-SCREEN_MSG_CNT;
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

	bt816_cmd_dl(DL_COLOR_RGB | BLUE);
	bt816_cmd_dl(DL_END);


	ertc_calendar_get(&dev_time);
	time_info time;
	time.date = dev_time.day;
	time.month = dev_time.month;
	time.year = dev_time.year;
	time.hour = dev_time.hour;
	time.min = dev_time.min;
	time.sec = dev_time.sec;
	print_date(message_text,time);
	bt816_cmd_text(60, 35, 1, 0, message_text);

	if(plc_can_link) {
		if(cl.pc21.ethip_state==1) bt816_cmd_text(300, 35, 1, 0, "\xd0\xa1\xd0\x9a\xd0\x90\xd0\x94\xd0\x90\x20\xd0\xbf\xd0\xbe\xd0\xb4\xd0\xba\xd0\xbb\xd1\x8e\xd1\x87\xd0\xb5\xd0\xbd\xd0\xb0");
		else {
			bt816_cmd_dl(DL_COLOR_RGB | RED);
			bt816_cmd_dl(DL_END);
			bt816_cmd_text(300, 35, 1, 0, "\xd0\xa1\xd0\x9a\xd0\x90\xd0\x94\xd0\x90\x20\xd0\x9d\xd0\x95\xd0\xa2\x20\xd0\xa1\xd0\x92\xd0\xaf\xd0\x97\xd0\x98");
			bt816_cmd_dl(DL_COLOR_RGB | BLUE);
			bt816_cmd_dl(DL_END);
		}
	}else {
		bt816_cmd_dl(DL_COLOR_RGB | RED);
		bt816_cmd_dl(DL_END);
		bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
		bt816_cmd_dl(VERTEX2F(290*16, 35*16));
		bt816_cmd_dl(VERTEX2F(425*16, 67*16));
		bt816_cmd_dl(DL_END);
		bt816_cmd_dl(DL_COLOR_RGB | BLUE);
		bt816_cmd_text(300, 35, 1, 0, "\xd0\x9d\xd0\xb5\x20\xd0\xb2\x20\xd1\x81\xd0\xb5\xd1\x82\xd0\xb8");
	}

	bt816_cmd_text(540, 35, 1, 0, appl_info_data.name);

	uint16_t step = 30;

	if(msg_cnt==0) first_visible_message_num = 0;

	if(msg_cnt) {
		for(int i=0;i<SCREEN_MSG_CNT;i++) {
			struct bit_message_conf conf;
			if(get_message_by_number(i+first_visible_message_num, &conf, message_text)) {

				if(conf.message_type==1) // warning
				{
					bt816_cmd_dl(DL_COLOR_RGB | YELLOW);
					bt816_cmd_dl(DL_END);
					bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
					bt816_cmd_dl(VERTEX2F(50*16, (90+i*step)*16));
					bt816_cmd_dl(VERTEX2F(750*16, (90+(i+1)*step)*16));
					bt816_cmd_dl(DL_END);
					bt816_cmd_dl(DL_COLOR_RGB | BLUE);
					bt816_cmd_dl(DL_END);
				}else if(conf.message_type==2) // alarm
				{
					bt816_cmd_dl(DL_COLOR_RGB | RED);
					bt816_cmd_dl(DL_END);
					bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
					bt816_cmd_dl(VERTEX2F(50*16, (90+i*step)*16));
					bt816_cmd_dl(VERTEX2F(750*16, (90+(i+1)*step)*16));
					bt816_cmd_dl(DL_END);
					bt816_cmd_dl(DL_COLOR_RGB | BLUE);
					bt816_cmd_dl(DL_END);
				}

				bt816_cmd_text(60, 90+i*step, 1, 0, message_text);
			}else break;
		}
	}else {
		if(plc_can_link) {
			bt816_cmd_dl(DL_COLOR_RGB | GREEN);
			bt816_cmd_dl(DL_END);
			bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
			bt816_cmd_dl(VERTEX2F(50*16, 90*16));
			bt816_cmd_dl(VERTEX2F(750*16, (90+step)*16));
			bt816_cmd_dl(DL_END);
			bt816_cmd_dl(DL_COLOR_RGB | BLUE);
			bt816_cmd_dl(DL_END);
			bt816_cmd_text(60, 90, 1, 0, "\xd0\xa1\xd0\x98\xd0\xa1\xd0\xa2\xd0\x95\xd0\x9c\xd0\x90\x20\xd0\x98\xd0\xa1\xd0\x9f\xd0\xa0\xd0\x90\xd0\x92\xd0\x9d\xd0\x90");
		}
	}
	bt816_cmd_text(290, 430, 1, 0, "\xd0\x94\xd0\xb5\xd0\xb2\xd0\xb8\xd1\x81\x20\xd0\x94\xd0\xb5\xd1\x80\xd0\xb1\xd0\xb8\x20\xd0\xa1\xd0\xb8\xd0\xb1\xd0\xb8\xd1\x80\xd1\x8c");

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
	if(key!=prev_key) {
		switch(key) {
			case KEY_0:
				prog_mode_flag = 1;
				init_prog_menu();
				current_menu = MENU_PROG;
				break;
			case KEY_RIGHT:
			case KEY_ENTER:
			case KEY_EXIT:
				current_menu = MENU_MAIN;
				break;
			case KEY_UP:
				if(first_visible_message_num>=SCREEN_MSG_CNT) first_visible_message_num-=SCREEN_MSG_CNT;
				else first_visible_message_num = 0;
				break;
			case KEY_DOWN:
				if(first_visible_message_num+SCREEN_MSG_CNT<msg_cnt) first_visible_message_num+=SCREEN_MSG_CNT;
				break;
		}
	}

	prev_key = key;
}

