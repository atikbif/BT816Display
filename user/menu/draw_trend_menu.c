/*
 * draw_trend_menu.c
 *
 *  Created on: 2023 May 27
 *      Author: pelev
 */

#include "draw_trend_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "trend_data.h"
#include "cur_time.h"

extern menu_list_t current_menu;
static uint8_t prev_key=0;
static uint8_t cur_trend = 0;
extern uint8_t tr_cnt;
static uint8_t min_val = 0;
static uint8_t max_val = 0;

static trend_point point_buf[TREND_SIZE];

static uint16_t get_trend_data_cnt(uint8_t num) {
	uint16_t cnt = 0;
	trend *ptr = get_trend_by_num(num);
	if(ptr) {
		int pos = ptr->first_point;

		for(int i=0;i<TREND_SIZE-1;i++) {
			if(ptr->point[pos].time!=0xFFFFFFFF) {
				cnt++;
				pos++;
				if(pos>=TREND_SIZE) pos = 0;
			}else break;
		}
	}
	return cnt;
}

static float get_point_time_pos( uint32_t point_time, uint32_t min_time, uint32_t max_time) {
	float res = (float)(point_time-min_time)/(max_time-min_time)*100;
	if(res<0) res = 0;
	if(res>100) res = 100;
	return res;
}

static float get_point_value_pos(uint32_t point_value, uint32_t min_value, uint32_t max_value) {
	float res = (float)(point_value-min_value)/(max_value-min_value)*100;
	if(res<0) res = 0;
	if(res>100) res = 100;
	return res;
}

static uint8_t get_trend_cur_value(uint8_t num) {
	trend *ptr = get_trend_by_num(num);
	if(ptr) {
		uint16_t cnt = get_trend_data_cnt(num);
		if(cnt) {
			int pos = ptr->first_point;
			for(int i=0;i<cnt;i++) {
				if(i==cnt-1) return ptr->point[pos].data;
				pos++;
				if(pos>=TREND_SIZE) pos = 0;
			}
		}
	}
	return 0;
}

static uint8_t get_trend_min(uint8_t num) {
	trend *ptr = get_trend_by_num(num);
	if(ptr) {
		uint16_t cnt = get_trend_data_cnt(num);
		uint8_t value = 255;
		int pos = ptr->first_point;
		for(int i=0;i<cnt;i++) {
			if(ptr->point[pos].data<value) value = ptr->point[pos].data;
			pos++;
			if(pos>=TREND_SIZE) pos = 0;
		}
		return value;
	}
	return 0;
}

static uint8_t get_trend_max(uint8_t num) {
	trend *ptr = get_trend_by_num(num);
	if(ptr) {
		uint16_t cnt = get_trend_data_cnt(num);
		uint8_t value = 0;
		int pos = ptr->first_point;
		for(int i=0;i<cnt;i++) {
			if(ptr->point[pos].data>value) value = ptr->point[pos].data;
			pos++;
			if(pos>=TREND_SIZE) pos = 0;
		}
		if(value<=250) value+=5;
		return value;
	}
	return 0;
}

static uint32_t get_trend_begin_time(uint8_t num) {
	trend *ptr = get_trend_by_num(num);
	if(ptr) {
		return ptr->point[ptr->first_point].time;
	}
	return 0;
}

static uint32_t get_trend_end_time(uint8_t num) {
	trend *ptr = get_trend_by_num(num);
	if(ptr) {
		uint16_t cnt = get_trend_data_cnt(num);
		if(cnt) {
			int pos = ptr->first_point;
			for(int i=0;i<cnt;i++) {
				if(i==cnt-1) return ptr->point[pos].time;
				pos++;
				if(pos>=TREND_SIZE) pos = 0;
			}
		}
	}
	return 0;
}

static void draw_time(uint16_t x, uint16_t y, uint8_t font, uint32_t tm){
	uint32_t ace;
	uint8_t b,d,m;

	ace = (tm/86400) + 32044 + JD0;
	b = (4*ace + 3)/146097;
	ace = ace - ((146097*b)/4);
	d = (4*ace + 3)/1461;
	ace = ace - ((1461*d)/4);
	m = (5*ace + 2)/153;
	uint8_t date = ace - ((153*m+2)/5) + 1;
	uint8_t month = m + 3 - (12*(m/10));
	uint8_t year = 100*b + d-4800 + (m/10) - 2000;
	uint8_t hour = (tm/3600)%24;
	uint8_t min = (tm/60)%60;
	uint8_t sec = (tm%60);
	uint32_t val[6];
	val[0] = hour;
	val[1] = min;
	val[2] = sec;
	val[3] = date;
	val[4] = month;
	val[5] = year;
	bt816_cmd_text_var(x, y, font, OPT_FORMAT, "%d:%d:%d %d/%d/%d", 6, val);
}

static void draw_trend(uint8_t num) {
	bt816_cmd_dl(DL_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_END);
	bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
	bt816_cmd_dl(VERTEX2F(95*16, 100*16));
	bt816_cmd_dl(VERTEX2F(695*16, 400*16));
	bt816_cmd_dl(DL_END);
	for(int i=0;i<3;i++) {
		bt816_cmd_dl(DL_BEGIN | BT816_LINES);
		bt816_cmd_dl(VERTEX2F(95*16, (100+i*150)*16));
		bt816_cmd_dl(VERTEX2F(75*16, (100+i*150)*16));
		bt816_cmd_dl(DL_END);
	}

	for(int i=0;i<11;i++) {
		bt816_cmd_dl(DL_BEGIN | BT816_LINES);
		bt816_cmd_dl(VERTEX2F((95+i*60)*16, 400*16));
		bt816_cmd_dl(VERTEX2F((95+i*60)*16, 420*16));
		bt816_cmd_dl(DL_END);
	}

	uint32_t min_time = get_trend_begin_time(num);
	uint32_t max_time = get_trend_end_time(num);
	if(max_time<=min_time) max_time = min_time+1;

	draw_time(30,440,22,min_time);
	draw_time(640,440,22,max_time);

	min_val = get_trend_min(num);
	max_val = get_trend_max(num);
	if(min_val==255) min_val = 254;
	if(max_val<=min_val) max_val = min_val+1;

	bt816_cmd_number(20, 85, 24, 0, max_val);
	bt816_cmd_number(20, 385, 24, 0, min_val);

	uint8_t alarm_flag = 0;
	uint8_t warn_flag = 0;
	uint8_t cur_val = get_trend_cur_value(num);
	trend *ptr = get_trend_by_num(num);
	if(ptr) {
		if(cur_val>ptr->max_alarm) alarm_flag = 1;
		if(cur_val<ptr->min_alarm) alarm_flag = 1;
		if(cur_val>ptr->max_warn) warn_flag = 1;
		if(cur_val<ptr->min_warn) warn_flag = 1;

		if(alarm_flag) {
			bt816_cmd_dl(DL_COLOR_RGB | RED);
			bt816_cmd_dl(DL_END);
		}else if(warn_flag) {
			bt816_cmd_dl(DL_COLOR_RGB | YELLOW);
			bt816_cmd_dl(DL_END);
		}else {
			bt816_cmd_dl(DL_COLOR_RGB | GREEN);
			bt816_cmd_dl(DL_END);
		}

		bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
		bt816_cmd_dl(VERTEX2F(150*16, 30*16));
		bt816_cmd_dl(VERTEX2F(210*16, 60*16));
		bt816_cmd_dl(DL_END);

		bt816_cmd_dl(DL_COLOR_RGB | BLACK);
		bt816_cmd_dl(DL_END);

		bt816_cmd_number(160, 30, 24, 0, get_trend_cur_value(num));

		bt816_cmd_dl(DL_COLOR_RGB | WHITE);
		bt816_cmd_dl(DL_END);

		uint16_t cnt = get_trend_data_cnt(num);
		if(cnt>TREND_SIZE) cnt = TREND_SIZE;
		int pos = ptr->first_point;

		for(int i=0;i<cnt;i++) {
			point_buf[i].data = ptr->point[pos].data;
			point_buf[i].time = ptr->point[pos].time;
			pos++;
			if(pos>=TREND_SIZE) pos = 0;
		}
		bt816_cmd_dl(DL_BEGIN | BT816_LINE_STRIP);
		uint16_t prev_value = 0;
		for(int i=0;i<cnt;i++) {
			if(i) {
				bt816_cmd_dl(VERTEX2F((95+get_point_time_pos(point_buf[i].time, min_time, max_time)*6)*16,
									prev_value));
			}
			bt816_cmd_dl(VERTEX2F((95+get_point_time_pos(point_buf[i].time, min_time, max_time)*6)*16,
					(400-get_point_value_pos(point_buf[i].data, min_val, max_val)*3)*16));
			prev_value = (400-get_point_value_pos(point_buf[i].data, min_val, max_val)*3)*16;
		}
		bt816_cmd_dl(DL_END);
	}

}

void init_draw_trend_menu() {
	cur_trend = 0;
}

void draw_trend_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	bt816_cmd_dl(DL_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(2,MEM_FONT22,0);
	bt816_cmd_setfont2(1,MEM_FONT40,0);

	if(tr_cnt==0) {
		bt816_cmd_text(300,200,2,0,"\xd0\x94\xd0\xbe\xd0\xb1\xd0\xb0\xd0\xb2\xd1\x8c\xd1\x82\xd0\xb5\x20\xd1\x82\xd1\x80\xd0\xb5\xd0\xbd\xd0\xb4");
	}else {
		uint32_t v = cur_trend+1;
		bt816_cmd_text_var(30, 30, 2, OPT_FORMAT, "\xd0\xa2\xd1\x80\xd0\xb5\xd0\xbd\xd0\xb4\x20%d", 1, &v);
		draw_trend(cur_trend+1);
	}
	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key!=prev_key) {
		switch(key) {
			case KEY_LEFT:
			case KEY_ENTER:
			case KEY_EXIT:
				current_menu = MENU_MAIN;
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
