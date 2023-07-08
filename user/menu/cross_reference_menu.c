/*
 * cross_reference_menu.c
 *
 *  Created on: 2023 May 23
 *      Author: pelev
 */

#include "cross_reference_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "cluster_state.h"
#include "config.h"

static uint8_t prev_key=0;
static const int max_records_cnt = 10;
static int first_visible_record = 0;
static uint8_t rec_buf[41];
static uint8_t io_buf[5];
static uint16_t rec_cnt;

static uint8_t down_sign = 0;
static uint8_t up_sign = 0;

extern menu_list_t current_menu;

void cross_reference_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x190000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(1,MEM_FONT22,0);
	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	rec_cnt = get_inp_cnt_with_descr(1) + get_out_cnt_with_descr(1);

	if(first_visible_record) up_sign=1;else up_sign=0;
	if(first_visible_record+max_records_cnt<rec_cnt) down_sign=1;else down_sign=0;

	uint8_t num = 0;
	for(int i=0;i<PC21_INP_CNT;i++) {
		for(int j=0;j<sizeof(rec_buf);j++) rec_buf[j]=0;
		uint16_t length = get_inp_description(1, i, rec_buf, io_buf);
		if(length) {
			if(num>=first_visible_record && (num<(first_visible_record+max_records_cnt))) {
				bt816_cmd_text(50, 95+(num-first_visible_record)*30, 1, 0, rec_buf);
				bt816_cmd_text(350, 95+(num-first_visible_record)*30, 1, 0, io_buf);
			}
			num++;
		}
	}

	for(int i=0;i<PC21_OUT_CNT;i++) {
		for(int j=0;j<sizeof(rec_buf);j++) rec_buf[j]=0;
		uint16_t length = get_out_description(1, i, rec_buf);
		if(length) {
			for(int j=0;j<sizeof(io_buf);j++) io_buf[j] = 0;
			io_buf[0] = '1';
			io_buf[1] = 'R';
			io_buf[2] = (i+1)/10 + '0';
			io_buf[3] = (i+1)%10 + '0';
			if(num>=first_visible_record && (num<(first_visible_record+max_records_cnt))) {
				bt816_cmd_text(50, 95+(num-first_visible_record)*30, 1, 0, rec_buf);
				bt816_cmd_text(350, 95+(num-first_visible_record)*30, 1, 0, io_buf);
			}
			num++;
		}
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

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key!=prev_key) {
		switch(key) {
			case KEY_LEFT:
				current_menu = MENU_DIAGNOSTIC2;
				break;
			case KEY_UP:
				if(first_visible_record) first_visible_record--;
				else first_visible_record = 0;
				break;
			case KEY_DOWN:
				if(first_visible_record+max_records_cnt<rec_cnt) first_visible_record++;
				break;
		}
	}
	prev_key = key;
}
