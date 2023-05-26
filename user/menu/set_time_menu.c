/*
 * set_time_menu.c
 *
 *  Created on: 2023 May 26
 *      Author: pelev
 */

#include "set_time_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "config.h"
#include "at32f435_437_board.h"

#define PARAM_CNT		6

enum {HOUR_LINK, MIN_LINK, SEC_LINK, DATE_LINK, MONTH_LINK, YEAR_LINK};

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

extern ertc_time_type dev_time;

static const uint16_t curs_width = 23;
static const uint16_t curs_height = 46;
static uint8_t prev_key=0;
extern menu_list_t current_menu;

static uint8_t pos = 1;

void init_time_menu() {
	ertc_calendar_get(&dev_time);

	params[0].max_val = 23;
	params[0].min_val = 0;
	params[0].width = 2;
	params[0].link = HOUR_LINK;
	params[0].value = dev_time.hour;
	params[0].x = 270;
	params[0].y = 150;
	params[0].edit_flag = 1;

	params[1].max_val = 59;
	params[1].min_val = 0;
	params[1].width = 2;
	params[1].link = MIN_LINK;
	params[1].value = dev_time.min;
	params[1].x = 370;
	params[1].y = 150;
	params[1].edit_flag = 0;

	params[2].value = dev_time.sec;
	params[2].max_val = 59;
	params[2].min_val = 0;
	params[2].width = 2;
	params[2].link = SEC_LINK;
	params[2].value = dev_time.sec;
	params[2].x = 470;
	params[2].y = 150;
	params[2].edit_flag = 0;

	params[3].value = dev_time.day;
	params[3].max_val = 31;
	params[3].min_val = 1;
	params[3].width = 2;
	params[3].link = DATE_LINK;
	params[3].value = dev_time.day;
	params[3].x = 270;
	params[3].y = 250;
	params[3].edit_flag = 0;

	params[4].value = dev_time.month;
	params[4].max_val = 12;
	params[4].min_val = 1;
	params[4].width = 2;
	params[4].link = MONTH_LINK;
	params[4].value = dev_time.month;
	params[4].x = 370;
	params[4].y = 250;
	params[4].edit_flag = 0;

	params[5].value = dev_time.year;
	params[5].max_val = 99;
	params[5].min_val = 0;
	params[5].width = 2;
	params[5].link = YEAR_LINK;
	params[5].value = dev_time.year;
	params[5].x = 470;
	params[5].y = 250;
	params[5].edit_flag = 0;

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

static void update_date_time_value(uint8_t cur_par, uint8_t num) {
	ertc_calendar_get(&dev_time);
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
		case HOUR_LINK:
			dev_time.hour = new_value;
			break;
		case MIN_LINK:
			dev_time.min = new_value;
			break;
		case SEC_LINK:
			dev_time.sec = new_value;
			break;
		case DATE_LINK:
			dev_time.day = new_value;
			break;
		case MONTH_LINK:
			dev_time.month = new_value;
			break;
		case YEAR_LINK:
			dev_time.year = new_value;
			break;
	}
	params[cur_par].value = new_value;
	ertc_date_set(dev_time.year, dev_time.month, dev_time.day, 1);
	ertc_time_set(dev_time.hour, dev_time.min, dev_time.sec, ERTC_24H);
	pos_to_right();
}

static void update_time_menu(uint8_t font) {
	ertc_calendar_get(&dev_time);
	for(int i=0;i<PARAM_CNT;i++) {
		switch(params[i].link) {
			case HOUR_LINK:
				if(params[i].edit_flag==0) params[i].value = dev_time.hour;
				break;
			case MIN_LINK:
				if(params[i].edit_flag==0) params[i].value = dev_time.min;
				break;
			case SEC_LINK:
				if(params[i].edit_flag==0) params[i].value = dev_time.sec;
				break;
			case DATE_LINK:
				if(params[i].edit_flag==0) params[i].value = dev_time.day;
				break;
			case MONTH_LINK:
				if(params[i].edit_flag==0) params[i].value = dev_time.month;
				break;
			case YEAR_LINK:
				if(params[i].edit_flag==0) params[i].value = dev_time.year;
				break;
		}
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

void set_time_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1C0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);


	bt816_cmd_setfont2(3,MEM_FONT30,0);
	bt816_cmd_setfont2(2,MEM_FONT22,0);

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(1,MEM_FONT40,0);
	bt816_cmd_setfont2(2,MEM_FONT30,0);

	update_time_menu(1);

	switch(key) {
		case KEY_LEFT:
		case KEY_ENTER:
			current_menu = MENU_CONFIG;
			break;
		case KEY_RIGHT:
			pos_to_right();
			break;
		case KEY_0:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 0);
			break;
		case KEY_1:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 1);
			break;
		case KEY_2:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 2);
			break;
		case KEY_3:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 3);
			break;
		case KEY_4:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 4);
			break;
		case KEY_5:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 5);
			break;
		case KEY_6:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 6);
			break;
		case KEY_7:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 7);
			break;
		case KEY_8:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 8);
			break;
		case KEY_9:
			if(prev_key!=key) update_date_time_value(get_cur_param(), 9);
			break;
	}
	prev_key = key;

	bt816_cmd_text(260, 190, 2, 0, "\xd1\x87\xd0\xb0\xd1\x81\xd1\x8b");
	bt816_cmd_text(367, 190, 2, 0, "\xd0\xbc\xd0\xb8\xd0\xbd");
	bt816_cmd_text(467, 190, 2, 0, "\xd1\x81\xd0\xb5\xd0\xba");

	bt816_cmd_text(250, 290, 2, 0, "\xd1\x87\xd0\xb8\xd1\x81\xd0\xbb\xd0\xbe");
	bt816_cmd_text(350, 290, 2, 0, "\xd0\xbc\xd0\xb5\xd1\x81\xd1\x8f\xd1\x86");
	bt816_cmd_text(467, 290, 2, 0, "\xd0\xb3\xd0\xbe\xd0\xb4");

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

}
