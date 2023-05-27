/*
 * add_trend_menu.c
 *
 *  Created on: 2023 May 27
 *      Author: pelev
 */

#include "add_trend_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "trend_data.h"

extern menu_list_t current_menu;
static uint8_t prev_key=0;

extern uint8_t tr_cnt;

static trend tr;

#define PARAM_CNT		6

enum {DEV_ADDR, INP_NUM, MIN_WARN, MIN_ALARM, MAX_WARN, MAX_ALARM};

typedef struct {
	uint16_t value;
	uint16_t max_val;
	uint16_t min_val;
	uint16_t width;
	uint16_t link;
	uint16_t edit_flag;
	uint16_t x;
	uint16_t y;
}edit_param;

edit_param params[PARAM_CNT];

static const uint16_t curs_width = 12;
static const uint16_t curs_height = 28;

static uint8_t pos = 1;


void init_add_trend_menu() {
	tr = get_new_trend();

	params[0].max_val = 255;
	params[0].min_val = 0;
	params[0].width = 3;
	params[0].link = DEV_ADDR;
	params[0].value = tr.dev_addr;
	params[0].x = 170;
	params[0].y = 145;
	params[0].edit_flag = 1;

	params[1].max_val = 14;
	params[1].min_val = 1;
	params[1].width = 2;
	params[1].link = INP_NUM;
	params[1].value = tr.inp_num;
	params[1].x = 550;
	params[1].y = 214;
	params[1].edit_flag = 0;

	params[5].max_val = 255;
	params[5].min_val = 0;
	params[5].width = 3;
	params[5].link = MIN_WARN;
	params[5].value = tr.min_warn;
	params[5].x = 550;
	params[5].y = 342;
	params[5].edit_flag = 0;

	params[4].max_val = 255;
	params[4].min_val = 0;
	params[4].width = 3;
	params[4].link = MIN_ALARM;
	params[4].value = tr.min_alarm;
	params[4].x = 550;
	params[4].y = 310;
	params[4].edit_flag = 0;

	params[3].max_val = 255;
	params[3].min_val = 0;
	params[3].width = 3;
	params[3].link = MAX_WARN;
	params[3].value = tr.max_warn;
	params[3].x = 550;
	params[3].y = 278;
	params[3].edit_flag = 0;

	params[2].max_val = 255;
	params[2].min_val = 0;
	params[2].width = 3;
	params[2].link = MAX_ALARM;
	params[2].value = tr.max_alarm;
	params[2].x = 550;
	params[2].y = 246;
	params[2].edit_flag = 0;

	pos = 1;
}

static uint8_t get_cur_param() {
	uint8_t cur_par = 0;
	for(int i=0;i<PARAM_CNT;i++) {
		if(params[i].edit_flag) {
			cur_par = i;
			break;
		}
	}
	return cur_par;
}

static void pos_to_right() {
	uint8_t cur_par = get_cur_param();
	params[cur_par].edit_flag = 1;
	if(pos==0) pos=1;
	if(pos>params[cur_par].width) pos = params[cur_par].width;
	if(pos<params[cur_par].width) pos++;
	else {
		params[cur_par].edit_flag = 0;
		cur_par++;
		if(cur_par>=PARAM_CNT) cur_par=0;
		params[cur_par].edit_flag = 1;
		pos = 1;
	}
}

static void update_param_value(uint8_t cur_par, uint8_t num) {
	if(cur_par>=PARAM_CNT) return;
	uint16_t new_value = params[cur_par].value;
	uint16_t numbers[3] = {0,0,0};
	while(new_value>=100) {
		new_value-=100;
		numbers[0]++;
	}
	while(new_value>=10) {
		new_value-=10;
		numbers[1]++;
	}
	numbers[2] = new_value;
	uint8_t index = (3-params[cur_par].width)+(pos-1);
	if(index>=3) return;
	numbers[index] = num;
	new_value = numbers[0]*100 + numbers[1]*10 + numbers[2];
	if(new_value<params[cur_par].min_val) new_value = params[cur_par].min_val;
	if(new_value>params[cur_par].max_val) new_value = params[cur_par].max_val;
	switch(params[cur_par].link) {
		case DEV_ADDR:
			tr.dev_addr = new_value;
			break;
		case INP_NUM:
			tr.inp_num = new_value;
			break;
		case MIN_WARN:
			tr.min_warn = new_value;
			break;
		case MIN_ALARM:
			tr.min_alarm = new_value;
			break;
		case MAX_WARN:
			tr.max_warn = new_value;
			break;
		case MAX_ALARM:
			tr.max_alarm = new_value;
			break;
	}
	params[cur_par].value = new_value;
	pos_to_right();
}

static void update_data(uint8_t font) {
	for(int i=0;i<PARAM_CNT;i++) {
		if(params[i].edit_flag) {
			if(pos>params[i].width) pos = params[i].width;
			bt816_cmd_dl(DL_COLOR_RGB | 0x808080UL);
			bt816_cmd_dl(DL_END);
			bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
			bt816_cmd_dl(VERTEX2F((params[i].x+(pos-1)*curs_width)*16, (params[i].y+2)*16));
			bt816_cmd_dl(VERTEX2F((params[i].x+pos*curs_width)*16, (params[i].y+curs_height)*16));
			bt816_cmd_dl(DL_END);
		}
		bt816_cmd_dl(DL_COLOR_RGB | WHITE);
		bt816_cmd_dl(DL_END);
		uint32_t val = params[i].value;
		if(params[i].width==1) {
			bt816_cmd_text_var(params[i].x, params[i].y, font, OPT_FORMAT, "%1d", 1, &val);
		}else if(params[i].width==2) {
			bt816_cmd_text_var(params[i].x, params[i].y, font, OPT_FORMAT, "%2d", 1, &val);
		}else if(params[i].width==3) {
			bt816_cmd_text_var(params[i].x, params[i].y, font, OPT_FORMAT, "%3d", 1, &val);
		}
	}
}

void add_trend_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1F0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(1,MEM_FONT22,0);
	bt816_cmd_setfont2(2,MEM_FONT40,0);


	if(tr_cnt>=get_trend_max_cnt()) {
		bt816_cmd_dl(DL_COLOR_RGB | RED);
		bt816_cmd_dl(DL_END);
		bt816_cmd_text(150,145,1,0,"FULL");

		switch(key) {
			case KEY_LEFT:
			case KEY_EXIT:
			case KEY_DOWN:
			case KEY_ENTER:
				current_menu = MENU_TREND_CONF;
				break;
		}
		prev_key = key;
	}else {
		bt816_cmd_dl(DL_COLOR_RGB | WHITE);
		bt816_cmd_dl(DL_END);

		update_data(1);

		switch(key) {
			case KEY_LEFT:
			case KEY_EXIT:
				current_menu = MENU_TREND_CONF;
				break;
			case KEY_DOWN:
			case KEY_ENTER:
				current_menu = MENU_TREND_CONF;
				add_trend(tr);
				save_trends();
				break;
			case KEY_RIGHT:
				pos_to_right();
				break;
			case KEY_0:
				if(prev_key!=key) update_param_value(get_cur_param(), 0);
				break;
			case KEY_1:
				if(prev_key!=key) update_param_value(get_cur_param(), 1);
				break;
			case KEY_2:
				if(prev_key!=key) update_param_value(get_cur_param(), 2);
				break;
			case KEY_3:
				if(prev_key!=key) update_param_value(get_cur_param(), 3);
				break;
			case KEY_4:
				if(prev_key!=key) update_param_value(get_cur_param(), 4);
				break;
			case KEY_5:
				if(prev_key!=key) update_param_value(get_cur_param(), 5);
				break;
			case KEY_6:
				if(prev_key!=key) update_param_value(get_cur_param(), 6);
				break;
			case KEY_7:
				if(prev_key!=key) update_param_value(get_cur_param(), 7);
				break;
			case KEY_8:
				if(prev_key!=key) update_param_value(get_cur_param(), 8);
				break;
			case KEY_9:
				if(prev_key!=key) update_param_value(get_cur_param(), 9);
				break;
		}
		prev_key = key;
	}





	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
}

