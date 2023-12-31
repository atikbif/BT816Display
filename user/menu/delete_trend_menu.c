/*
 * delete_trend_menu.c
 *
 *  Created on: 2023 May 27
 *      Author: pelev
 */

#include "delete_trend_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "config.h"
#include <string.h>
#include "trend_data.h"

extern menu_list_t current_menu;
static uint8_t prev_key=0;

static uint8_t cur_trend = 0;

extern uint8_t tr_cnt;

void init_delete_trend_info_menu() {
	cur_trend = 0;
}

void delete_trend_info_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1C0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(2,MEM_FONT22,0);
	bt816_cmd_setfont2(1,MEM_FONT30,0);

	if(tr_cnt==0) {
		bt816_cmd_text(300,200,2,0,"\xd0\x94\xd0\xbe\xd0\xb1\xd0\xb0\xd0\xb2\xd1\x8c\xd1\x82\xd0\xb5\x20\xd1\x82\xd1\x80\xd0\xb5\xd0\xbd\xd0\xb4");
	}else {

		bt816_cmd_fgcolor(COLOR_RGB(100, 100, 130));
		bt816_cmd_button(60, 100+cur_trend*30, 120, 32, 2, 0, "");

		uint32_t v = 0;
		for(int i=0;i<tr_cnt;i++) {
			v = i+1;
			bt816_cmd_text_var(70, 100+i*30, 2, OPT_FORMAT, "\xd0\xa2\xd1\x80\xd0\xb5\xd0\xbd\xd0\xb4\x20%d", 1, &v);
		}

		trend * ptr = get_trend_by_num(cur_trend+1);
		if(ptr) {
			v = ptr->dev_addr;
			bt816_cmd_text_var(400, 95, 1, OPT_FORMAT, "\xd0\xa3\xd0\xb7\xd0\xb5\xd0\xbb\x20%d", 1, &v);
			v = ptr->inp_num;
			bt816_cmd_text_var(400, 135, 1, OPT_FORMAT, "\xd0\x92\xd0\xb2\xd0\xbe\xd0\xb4\x20%d", 1, &v);
			v = ptr->max_alarm;
			bt816_cmd_text_var(400, 175, 1, OPT_FORMAT, "HA %d", 1, &v);
			v = ptr->max_warn;
			bt816_cmd_text_var(400, 215, 1, OPT_FORMAT, "HW %d", 1, &v);
			v = ptr->min_alarm;
			bt816_cmd_text_var(400, 255, 1, OPT_FORMAT, "LA %d", 1, &v);
			v = ptr->min_warn;
			bt816_cmd_text_var(400, 295, 1, OPT_FORMAT, "LW %d", 1, &v);
		}
		bt816_cmd_text(100, 345, 1, 0, "\xd0\x94\xd0\xbb\xd1\x8f\x20\xd1\x83\xd0\xb4\xd0\xb0\xd0\xbb\xd0\xb5\xd0\xbd\xd0\xb8\xd1\x8f\x20\xd1\x82\xd1\x80\xd0\xb5\xd0\xbd\xd0\xb4\xd0\xb0\x20\xd0\xbd\xd0\xb0\xd0\xb6\xd0\xbc\xd0\xb8\xd1\x82\xd0\xb5\x20\x45\x4e\x54\x45\x52");
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key!=prev_key) {
		switch(key) {
			case KEY_ENTER:
				delete_trend(cur_trend+1);
				save_trends();
				cur_trend = 0;
				break;
			case KEY_LEFT:
			case KEY_EXIT:
				current_menu = MENU_TREND_CONF;
				break;
			case KEY_UP:
				if(cur_trend) cur_trend--;
				break;
			case KEY_DOWN:
				if(cur_trend+1<tr_cnt) cur_trend++;
				break;
		}
	}
	prev_key = key;
}
