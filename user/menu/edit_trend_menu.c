/*
 * edit_trend_menu.c
 *
 *  Created on: 2023 May 27
 *      Author: pelev
 */

#include "edit_trend_menu.h"
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

#define PARAM_CNT		6

static trend tmp_trends[TREND_MAX_CNT];

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

static void update_params_by_trend_num(uint8_t num) {
	if(num>=TREND_MAX_CNT) return;
	trend *ptr = &tmp_trends[cur_trend];

	params[0].max_val = 255;
	params[0].min_val = 0;
	params[0].width = 3;
	params[0].link = DEV_ADDR;
	params[0].value = ptr->dev_addr;
	params[0].x = 480;
	params[0].y = 95;
	params[0].edit_flag = 1;

	params[1].max_val = 14;
	params[1].min_val = 1;
	params[1].width = 2;
	params[1].link = INP_NUM;
	params[1].value = ptr->inp_num;
	params[1].x = 480;
	params[1].y = 145;
	params[1].edit_flag = 0;

	params[2].max_val = 255;
	params[2].min_val = 0;
	params[2].width = 3;
	params[2].link = MAX_ALARM;
	params[2].value = ptr->max_alarm;
	params[2].x = 480;
	params[2].y = 195;
	params[2].edit_flag = 0;

	params[3].max_val = 255;
	params[3].min_val = 0;
	params[3].width = 3;
	params[3].link = MAX_WARN;
	params[3].value = ptr->max_warn;
	params[3].x = 480;
	params[3].y = 245;
	params[3].edit_flag = 0;

	params[4].max_val = 255;
	params[4].min_val = 0;
	params[4].width = 3;
	params[4].link = MIN_ALARM;
	params[4].value = ptr->min_alarm;
	params[4].x = 480;
	params[4].y = 295;
	params[4].edit_flag = 0;

	params[5].max_val = 255;
	params[5].min_val = 0;
	params[5].width = 3;
	params[5].link = MIN_WARN;
	params[5].value = ptr->min_warn;
	params[5].x = 480;
	params[5].y = 345;
	params[5].edit_flag = 0;
}

void init_edit_trend_menu() {
	cur_trend = 0;
	for(int i=0;i<TREND_MAX_CNT;i++) {
		tmp_trends[i] = *get_trend_by_num(i+1);
	}
	update_params_by_trend_num(cur_trend);
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
	trend *ptr = &tmp_trends[cur_trend];
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
			ptr->dev_addr = new_value;
			break;
		case INP_NUM:
			ptr->inp_num = new_value;
			break;
		case MIN_WARN:
			ptr->min_warn = new_value;
			break;
		case MIN_ALARM:
			ptr->min_alarm = new_value;
			break;
		case MAX_WARN:
			ptr->max_warn = new_value;
			break;
		case MAX_ALARM:
			ptr->max_alarm = new_value;
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

void edit_trend_menu(uint16_t key) {
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
	bt816_cmd_setfont2(1,MEM_FONT40,0);

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

			bt816_cmd_dl(DL_COLOR_RGB | WHITE);
			bt816_cmd_dl(DL_END);

			bt816_cmd_text(400, 95, 2, 0, "\xd0\xa3\xd0\xb7\xd0\xb5\xd0\xbb");
			bt816_cmd_text(400, 145, 2, 0, "\xd0\x92\xd0\xb2\xd0\xbe\xd0\xb4");
			bt816_cmd_text(400, 195, 2, 0, "HA");
			bt816_cmd_text(400, 245, 2, 0, "HW");
			bt816_cmd_text(400, 295, 2, 0, "LA");
			bt816_cmd_text(400, 345, 2, 0, "LW");

			update_data(2);
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key!=prev_key) {
		switch(key) {
			case KEY_LEFT:
			case KEY_ENTER:
				for(int i=0;i<TREND_MAX_CNT;i++) {
					*get_trend_by_num(i+1) = tmp_trends[i];
				}
				save_trends();
				current_menu = MENU_TREND_CONF;
				break;
			case KEY_EXIT:
				current_menu = MENU_TREND_CONF;
				break;
			case KEY_UP:
				if(cur_trend) cur_trend--;
				update_params_by_trend_num(cur_trend);
				break;
			case KEY_DOWN:
				if(cur_trend+1<tr_cnt) cur_trend++;
				update_params_by_trend_num(cur_trend);
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
	}
	prev_key = key;
}
