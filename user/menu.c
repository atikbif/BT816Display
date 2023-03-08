/*
 * menu.c
 *
 *  Created on: 2023 Mar 8
 *      Author: Roman
 */

#include "menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "mnemo.h"

uint8_t mnemo_num = 0;
extern uint16_t mnemo_cnt;

extern uint8_t plc_can_link;
extern uint8_t plc_eth_ip_state;
extern uint16_t plc_app_id;
extern uint8_t plc_can_addr;

const uint8_t message_help[] = "\xd0\xa1\xd0\x9f\xd0\xa0\xd0\x90\xd0\x92\xd0\x9a\xd0\x90\x20\x31";
const uint8_t message_plc[] = "\xd0\x9f\xd0\x9b\xd0\x9a\x20\x32";
const uint8_t message_mnemo[] = "\xd0\x9c\xd0\x9d\xd0\x95\xd0\x9c\xd0\x9e\xd0\xa1\xd0\xa5\xd0\x95\xd0\x9c\xd0\xab\x20\x33";
const uint8_t message_manage[] = "\xd0\xa3\xd0\x9f\xd0\xa0\xd0\x90\xd0\x92\xd0\x9b\xd0\x95\xd0\x9d\xd0\x98\xd0\x95\x20\x34";
const uint8_t message_diagn[] = "\xd0\x94\xd0\x98\xd0\x90\xd0\x93\xd0\x9d\xd0\x9e\xd0\xa1\xd0\xa2\xd0\x98\xd0\x9a\xd0\x90\x20\x35";

const uint8_t message_version[] = "\xd0\x92\xd0\xb5\xd1\x80\xd1\x81\xd0\xb8\xd1\x8f\x20\xd0\x9f\xd0\x9e\x20\xef\xbc\x9a\x31\x2e\x30";

#define HELP_STR_CNT	6

const char* help_data[HELP_STR_CNT] = {
		"\xd0\x92\xd0\xb5\xd1\x80\xd1\x81\xd0\xb8\xd1\x8f\x20\xd0\x9f\xd0\x9e\x20\x31\x2e\x30",
		"\xd0\xb2\xd0\xbb\xd0\xb5\xd0\xb2\xd0\xbe\x20\x2d\x20\xd0\xbf\xd1\x80\xd0\xb5\xd0\xb4\xd1\x8b\xd0\xb4\xd1\x83\xd1\x89\xd0\xb5\xd0\xb5\x20\xd0\xbc\xd0\xb5\xd0\xbd\xd1\x8e",
		"\xd0\xb2\xd0\xbf\xd1\x80\xd0\xb0\xd0\xb2\xd0\xbe\x20\x2d\x20\xd0\xbf\xd0\xbe\xd0\xb4\xd1\x82\xd0\xb2\xd0\xb5\xd1\x80\xd0\xb4\xd0\xb8\xd1\x82\xd1\x8c\x20\xd0\xb2\xd0\xb2\xd0\xbe\xd0\xb4",
		"\xd0\xb2\xd0\xb2\xd0\xb5\xd1\x80\xd1\x85\x20\x2d\x20\xd1\x81\xd0\xbc\xd0\xb5\xd1\x81\xd1\x82\xd0\xb8\xd1\x82\xd1\x8c\x20\xd0\xba\xd1\x83\xd1\x80\xd1\x81\xd0\xbe\xd1\x80\x20\xd0\xb2\xd0\xb2\xd0\xb5\xd1\x80\xd1\x85\x2f\xd0\xbf\xd1\x80\xd0\xb5\xd0\xb4\xd1\x8b\xd0\xb4\xd1\x83\xd1\x89\xd0\xb0\xd1\x8f\x20\xd0\xbc\xd0\xbd\xd0\xb5\xd0\xbc\xd0\xbe\xd1\x81\xd1\x85\xd0\xb5\xd0\xbc\xd0\xb0",
		"\xd0\xb2\xd0\xbd\xd0\xb8\xd0\xb7\x20\x2d\x20\xd1\x81\xd0\xbc\xd0\xb5\xd1\x81\xd1\x82\xd0\xb8\xd1\x82\xd1\x8c\x20\xd0\xba\xd1\x83\xd1\x80\xd1\x81\xd0\xbe\xd1\x80\x20\xd0\xb2\xd0\xbd\xd0\xb8\xd0\xb7\x2f\xd1\x81\xd0\xbb\xd0\xb5\xd0\xb4\xd1\x83\xd1\x8e\xd1\x89\xd0\xb0\xd1\x8f\x20\xd0\xbc\xd0\xbd\xd0\xb5\xd0\xbc\xd0\xbe\xd1\x81\xd1\x85\xd0\xb5\xd0\xbc\xd0\xb0",
		"\xd1\x81\xd0\xb1\xd1\x80\xd0\xbe\xd1\x81\x20\x2d\x20\xd0\xbe\xd1\x87\xd0\xb8\xd1\x81\xd1\x82\xd0\xb8\xd1\x82\xd1\x8c\x20\xd0\xbf\xd0\xbe\xd0\xbb\xd0\xb5\x20\xd0\xb2\xd0\xb2\xd0\xbe\xd0\xb4\xd0\xb0"};

const uint8_t message_plc_can_on[] = "\xd0\xbf\xd0\xbe\xd0\xb4\xd0\xba\xd0\xbb\xd1\x8e\xd1\x87\xd0\xb5\xd0\xbd";
const uint8_t message_plc_can_off[] = "\xd0\xbd\xd0\xb5\xd1\x82\x20\xd1\x81\xd0\xbe\xd0\xb5\xd0\xb4\xd0\xb8\xd0\xbd\xd0\xb5\xd0\xbd\xd0\xb8\xd1\x8f";
const uint8_t message_plc_scada_on[] = "\xd0\xbf\xd0\xbe\xd0\xb4\xd0\xba\xd0\xbb\xd1\x8e\xd1\x87\xd0\xb5\xd0\xbd\xd0\xb0";
const uint8_t message_plc_scada_off[] = "\xd0\xbd\xd0\xb5\xd1\x82\x20\xd1\x81\xd0\xbe\xd0\xb5\xd0\xb4\xd0\xb8\xd0\xbd\xd0\xb5\xd0\xbd\xd0\xb8\xd1\x8f";

enum MENU_LIST {MENU_MAIN, MENU_HELP, MENU_PC21, MENU_MNEMO, MENU_MANAGE, MENU_DIAGN, MENU_LIMIT};

typedef void(*f_ptr)(uint16_t);

static void main_menu(uint16_t key);
static void help_menu(uint16_t key);
static void pc21_menu(uint16_t key);
static void mnemo_menu(uint16_t key);
static void manage_menu(uint16_t key);
static void diagn_menu(uint16_t key);

const f_ptr screen[] = {
	main_menu,help_menu,pc21_menu,mnemo_menu,manage_menu,diagn_menu
};

enum MENU_LIST current_menu = MENU_MAIN;

void display_menu(uint16_t key) {
	if(current_menu>MENU_LIMIT) current_menu = MENU_MAIN;
	screen[current_menu](key);
}

void init_menu() {
	init_display_fonts();
}

void main_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	//bt816_cmd_setfont2(2,MEM_L1FONT22,0);
	//bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	bt816_cmd_setfont2(4,MEM_L1FONT40,0);

	uint16_t x_pos = 200;
	uint16_t y_pos = 75;
	uint16_t width = 400;
	uint16_t height = 50;
	uint16_t distance = 20;

	bt816_cmd_fgcolor(COLOR_RGB(100, 0, 0));
	bt816_cmd_button(x_pos, y_pos, width, height, 4, 0, message_help);
	bt816_cmd_button(x_pos, y_pos + height + distance, width, height, 4, 0, message_plc);
	bt816_cmd_button(x_pos, y_pos + (height + distance)*2, width, height, 4, 0, message_mnemo);
	bt816_cmd_button(x_pos, y_pos + (height + distance)*3, width, height, 4, 0, message_manage);
	bt816_cmd_button(x_pos, y_pos + (height + distance)*4, width, height, 4, 0, message_diagn);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_1:
			current_menu = MENU_HELP;
			break;
		case KEY_2:
			current_menu = MENU_PC21;
			break;
		case KEY_3:
			current_menu = MENU_MNEMO;
			init_mnemo();
			mnemo_num = 0;
			read_mnemo_data(mnemo_num);
			break;
		case KEY_4:
			break;
		case KEY_5:
			break;
	}
}

void help_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint16_t x_pos = 50;
	uint16_t y_pos = 50;
	uint16_t step = 50;

	bt816_cmd_dl(DL_COLOR_RGB | 0x00FFFFFF);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	bt816_cmd_setfont2(2,MEM_L1FONT22,0);
	bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	//bt816_cmd_setfont2(4,MEM_L1FONT40,0);

	for(uint8_t i=0;i<HELP_STR_CNT;i++) {
		uint8_t font = 2;
		if(i==0) font = 3;
		bt816_cmd_text(x_pos, y_pos + step*i, font, 0, help_data[i]);
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);


	if(key==KEY_LEFT) {
		current_menu = MENU_MAIN;
	}
}

void pc21_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	//bt816_cmd_setfont2(2,MEM_L1FONT22,0);
	bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	//bt816_cmd_setfont2(4,MEM_L1FONT40,0);

	uint16_t x_pos = 50;
	uint16_t y_pos = 50;
	uint16_t data_x_pos = 300;
	uint16_t step = 50;

	bt816_cmd_text(x_pos, y_pos, 3, 0, "CAN:");
	if(plc_can_link) bt816_cmd_text(data_x_pos, y_pos, 3, 0, message_plc_can_on);
	else bt816_cmd_text(data_x_pos, y_pos, 3, 0, message_plc_can_off);


	y_pos+=step;
	bt816_cmd_text(x_pos, y_pos, 3, 0, "SCADA:");
	if(plc_eth_ip_state==1) bt816_cmd_text(data_x_pos, y_pos, 3, 0, message_plc_scada_on);
	else bt816_cmd_text(data_x_pos, y_pos, 3, 0, message_plc_scada_off);

	y_pos+=step;
	bt816_cmd_text(x_pos, y_pos, 3, 0, "APP ID:");
	bt816_cmd_number(data_x_pos, y_pos, 3, 0, plc_app_id);

	y_pos+=step;
	bt816_cmd_text(x_pos, y_pos, 3, 0, "CAN ADDR:");
	bt816_cmd_number(data_x_pos, y_pos, 3, 0, plc_can_addr);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
	if(key==KEY_LEFT) {
		current_menu = MENU_MAIN;
	}
}

void mnemo_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
	if(mnemo_num<mnemo_cnt) {
		draw_mnemo();
	}else {
		bt816_cmd_fgcolor(COLOR_RGB(100, 0, 0));
		bt816_cmd_button(200, 100, 400, 50, 28, 0, "NO DATA");
	}
	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
	switch(key) {
		case KEY_LEFT:
			current_menu = MENU_MAIN;
			break;
		case KEY_UP:
			if(mnemo_num) mnemo_num--;
			else {
				if(mnemo_cnt) mnemo_num=mnemo_cnt-1;
			}
			read_mnemo_data(mnemo_num);
			break;
		case KEY_DOWN:
			if(mnemo_num<(mnemo_cnt-1)) mnemo_num++;else mnemo_num=0;
			read_mnemo_data(mnemo_num);
			break;
	}
}

void manage_menu(uint16_t key) {

}

void diagn_menu(uint16_t key) {

}
