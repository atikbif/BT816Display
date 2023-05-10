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
#include "plc_data.h"
#include "var_link.h"
#include <string.h>
#include "can.h"
#include "menu_list.h"
#include "appl_info_menu.h"
#include "test_menu.h"

uint8_t mnemo_num = 0;
extern uint16_t mnemo_cnt;

static uint16_t offset = 0;
static uint16_t curs_pos = 0;

extern uint8_t plc_can_link;
extern uint8_t plc_eth_ip_state;
extern uint16_t plc_app_id;
extern uint8_t plc_can_addr;

extern enum DI_STATE plc_di_state[DI_CNT];
extern enum DO_STATE plc_do_state[DO_CNT];
extern enum DI_TYPE plc_di_type[DI_CNT];
extern enum AI_TYPE plc_ai_type[AI_CNT];

extern uint16_t plc_ain_raw[AI_CNT];
extern uint8_t plc_clust_bits[CLUST_BITS_CNT];
extern uint16_t plc_clust_regs[CLUSTER_REGS_CNT];
extern uint8_t plc_net_bits[NET_BITS_CNT];
extern uint16_t plc_net_regs[NET_REGS_CNT];

const uint8_t message_help[] = "\xd0\xa1\xd0\x9f\xd0\xa0\xd0\x90\xd0\x92\xd0\x9a\xd0\x90\x20\x31";
const uint8_t message_plc[] = "\xd0\x9f\xd0\x9b\xd0\x9a\x20\x32";
const uint8_t message_mnemo[] = "\xd0\x9c\xd0\x9d\xd0\x95\xd0\x9c\xd0\x9e\xd0\xa1\xd0\xa5\xd0\x95\xd0\x9c\xd0\xab\x20\x33";
const uint8_t message_manage[] = "\xd0\xa3\xd0\x9f\xd0\xa0\xd0\x90\xd0\x92\xd0\x9b\xd0\x95\xd0\x9d\xd0\x98\xd0\x95\x20\x34";
const uint8_t message_diagn[] = "\xd0\x94\xd0\x98\xd0\x90\xd0\x93\xd0\x9d\xd0\x9e\xd0\xa1\xd0\xa2\xd0\x98\xd0\x9a\xd0\x90\x20\x35";

const uint8_t message_version[] = "\xd0\x92\xd0\xb5\xd1\x80\xd1\x81\xd0\xb8\xd1\x8f\x20\xd0\x9f\xd0\x9e\x20\xef\xbc\x9a\x31\x2e\x30";

#define HELP_STR_CNT	6

#define MAN_VAR_CNT		15
const char* man_var_names[MAN_VAR_CNT] = {
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x31",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x32",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x33",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x34",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x35",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x31",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x32",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x33",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x34",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x35",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x31",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x32",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x33",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x34",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x35"
};

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

const uint8_t message_not_used[] = "\xd0\xbd\xd0\xb5\x20\xd0\xb8\xd1\x81\xd0\xbf\xd0\xbe\xd0\xbb\xd1\x8c\xd0\xb7\xd1\x83\xd0\xb5\xd1\x82\xd1\x81\xd1\x8f";
const uint8_t message_on[] = "\xd0\x92\xd0\x9a\xd0\x9b";
const uint8_t message_off[] = "\xd0\x92\xd0\xab\xd0\x9a\xd0\x9b";
const uint8_t message_short_circ[] = "\xd0\x97\xd0\x90\xd0\x9c\xd0\xab\xd0\x9a\xd0\x90\xd0\x9d\xd0\x98\xd0\x95";
const uint8_t message_break[] = "\xd0\x9e\xd0\x91\xd0\xa0\xd0\xab\xd0\x92";


typedef void(*f_ptr)(uint16_t);

static void main_menu(uint16_t key);
static void help_menu(uint16_t key);
static void pc21_menu(uint16_t key);
static void mnemo_menu(uint16_t key);
static void manage_menu(uint16_t key);
static void diagn_menu(uint16_t key);
static void di_menu(uint16_t key);
static void do_menu(uint16_t key);
static void ai_menu(uint16_t key);
static void clust_bits_menu(uint16_t key);
static void net_bits_menu(uint16_t key);
static void clust_regs_menu(uint16_t key);
static void net_regs_menu(uint16_t key);
static void edit_u16_menu(uint16_t key);

typedef struct{
	uint8_t name[40];
	uint32_t min;
	uint32_t max;
	uint32_t value;
	uint16_t link_type;
	uint16_t link_index;
	uint16_t var_cnt;
}manage_var;

manage_var edit_var;
uint8_t u16var_text[6]="     ";
uint16_t var_x_pos = 0;

const f_ptr screen[] = {
	main_menu,appl_info_menu,help_menu,pc21_menu,mnemo_menu,manage_menu,diagn_menu,di_menu,
	do_menu,ai_menu,clust_bits_menu,net_bits_menu,clust_regs_menu,net_regs_menu,
	edit_u16_menu
};

menu_list_t current_menu = MENU_MAIN;

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

	uint32_t addr = 0x100000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	test_appl_info_menu();

//	bt816_cmd_setfont2(4,MEM_FONT40,0);
//
//	uint16_t x_pos = 200;
//	uint16_t y_pos = 75;
//	uint16_t width = 400;
//	uint16_t height = 50;
//	uint16_t distance = 20;
//
//	bt816_cmd_fgcolor(COLOR_RGB(100, 0, 0));
//	bt816_cmd_button(x_pos, y_pos, width, height, 4, 0, message_help);
//	bt816_cmd_button(x_pos, y_pos + height + distance, width, height, 4, 0, message_plc);
//	bt816_cmd_button(x_pos, y_pos + (height + distance)*2, width, height, 4, 0, message_mnemo);
//	bt816_cmd_button(x_pos, y_pos + (height + distance)*3, width, height, 4, 0, message_manage);
//	bt816_cmd_button(x_pos, y_pos + (height + distance)*4, width, height, 4, 0, message_diagn);

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
			current_menu = MENU_MANAGE;
			offset = 0;
			curs_pos = 0;
			break;
		case KEY_5:
			current_menu = MENU_DIAGN;
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
	bt816_cmd_setfont2(2,MEM_FONT22,0);
	bt816_cmd_setfont2(3,MEM_FONT30,0);
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
	bt816_cmd_setfont2(3,MEM_FONT30,0);
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

static uint8_t get_manage_var(uint16_t i, manage_var *var) {
	uint8_t res = 0;
	if(i<MAN_VAR_CNT) {
		res = 1;
		for(uint16_t j=0;j<40;j++) var->name[j] = 0;
		for(uint16_t j=0;j<40;j++) {
			if(j<strlen(man_var_names[i])) var->name[j] = man_var_names[i][j];
		}
		if(i<5) {
			var->link_type = VAR_LINK_CL_REG;
			var->link_index = i+10;
			var->min = 0;
			var->max = 15000;
		}else if(i<10) {
			var->link_type = VAR_LINK_CL_BIT;
			var->link_index = i-5;
			var->min = 0;
			var->max = 1;
		}else {
			var->link_type = VAR_LINK_NET_BIT;
			var->link_index = i-10;
			var->min = 0;
			var->max = 1;
		}
		var->var_cnt = MAN_VAR_CNT;
		var->value = 0;
		switch(var->link_type) {
			case VAR_LINK_CL_REG:
				if(var->link_index<CLUSTER_REGS_CNT) var->value = plc_clust_regs[var->link_index];
				break;
			case VAR_LINK_NET_REG:
				if(var->link_index<NET_REGS_CNT) var->value = plc_net_regs[var->link_index];
				break;
			case VAR_LINK_CL_BIT:
				if(var->link_index<CLUST_BITS_CNT) var->value = plc_clust_bits[var->link_index];
				break;
			case VAR_LINK_NET_BIT:
				if(var->link_index<NET_BITS_CNT) var->value = plc_net_bits[var->link_index];
				break;
		}
	}
	return res;
}

void write_var_to_plc(manage_var *var) {
	if(plc_can_link==0) return;
	if(var->value>var->max) var->value = var->max;
	if(var->value<var->min) var->value = var->min;
	switch(var->link_type) {
		case VAR_LINK_CL_REG:
			if(var->link_index<CLUSTER_REGS_CNT) {
				plc_clust_regs[var->link_index] = var->value;
				write_clust_reg(var->link_index, var->value);
			}
			break;
		case VAR_LINK_NET_REG:
			if(var->link_index<NET_REGS_CNT) {
				plc_net_regs[var->link_index] = var->value;
				write_net_reg(var->link_index, var->value);
			}
			break;
		case VAR_LINK_CL_BIT:
			if(var->link_index<CLUST_BITS_CNT) {
				plc_clust_bits[var->link_index] = var->value;
				write_clust_bit(var->link_index, var->value);
			}
			break;
		case VAR_LINK_NET_BIT:
			if(var->link_index<NET_BITS_CNT) {
				plc_net_bits[var->link_index] = var->value;
				write_net_bit(var->link_index, var->value);
			}
			break;
	}
}

void edit_u16_menu(uint16_t key) {
	uint32_t val = 0;
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	//bt816_cmd_setfont2(2,MEM_L1FONT22,0);
	bt816_cmd_setfont2(3,MEM_FONT30,0);
	bt816_cmd_setfont2(4,MEM_FONT40,0);

	bt816_cmd_text(50,50,3,0,"\xd0\x92\xd0\xb2\xd0\xb5\xd0\xb4\xd0\xb8\xd1\x82\xd0\xb5\x20\xd0\xb7\xd0\xbd\xd0\xb0\xd1\x87\xd0\xb5\xd0\xbd\xd0\xb8\xd0\xb5");
	bt816_cmd_text(50,90,3,0,"\xd0\xb2\xd0\xbd\xd0\xb8\xd0\xb7\x20\x2d\x20\xd1\x81\xd0\xbe\xd1\x85\xd1\x80\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x82\xd1\x8c");
	bt816_cmd_text(50,130,3,0,"\xd0\xb2\xd0\xbb\xd0\xb5\xd0\xb2\xd0\xbe\x20\x2d\x20\xd0\xb2\xd0\xb5\xd1\x80\xd0\xbd\xd1\x83\xd1\x82\xd1\x8c\xd1\x81\xd1\x8f");



	bt816_cmd_button(50, 200, 200, 50, 4, 0, u16var_text);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	uint16_t v = 0;
	for(uint8_t i=0;i<5;i++) {
		if((u16var_text[i]>='0') && (u16var_text[i]<'9')) {
			v = v*10 + (u16var_text[i] - '0');
		}else break;
	}
	edit_var.value = v;

	switch(key) {
		case KEY_1:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '1';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_2:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '2';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_3:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '3';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_4:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '4';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_5:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '5';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_6:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '6';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_7:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '7';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_8:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '8';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_9:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '9';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_0:
			if(var_x_pos==0) {
				u16var_text[0] = ' ';
				u16var_text[1] = ' ';
				u16var_text[2] = ' ';
				u16var_text[3] = ' ';
				u16var_text[4] = ' ';
				u16var_text[5] = 0;
			}
			u16var_text[var_x_pos] = '0';
			var_x_pos++;if(var_x_pos>=5) var_x_pos = 0;
			break;
		case KEY_LEFT:
			current_menu = MENU_MANAGE;
			break;
		case KEY_EXIT:
		case KEY_ENTER:
			u16var_text[0] = ' ';
			u16var_text[1] = ' ';
			u16var_text[2] = ' ';
			u16var_text[3] = ' ';
			u16var_text[4] = ' ';
			u16var_text[5] = 0;
			var_x_pos = 0;
			break;
		case KEY_RIGHT:
			write_var_to_plc(&edit_var);
			current_menu = MENU_MANAGE;
			break;
	}
}

void manage_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	bt816_cmd_setfont2(2,MEM_FONT22,0);
	//bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	bt816_cmd_setfont2(4,MEM_FONT40,0);

	uint16_t x_pos = 50;
	uint16_t y_pos = 50;
	uint16_t step = 30;
	uint16_t row = 14;
	uint16_t x_data_pos = 400;

	uint16_t var_cnt = 0;

	for(uint16_t i=0;i<row;i++) {
		if(i==curs_pos) {
			bt816_cmd_fgcolor(COLOR_RGB(100, 100, 130));
			bt816_cmd_button(x_pos-10, y_pos+i*step, x_data_pos-x_pos, step, 2, 0, "");
		}
		manage_var var;
		if(get_manage_var(offset+i, &var)) {
			bt816_cmd_text(x_pos, y_pos+i*step, 2, 0, var.name);
			bt816_cmd_number(x_data_pos, y_pos+i*step, 2, 0, var.value);
			var_cnt = var.var_cnt;
			if(i==curs_pos) edit_var = var;
		}
	}

	if(var_cnt==0) {
		bt816_cmd_text(x_pos,y_pos,4,0,"NO DATA");
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_UP:
			if(curs_pos) curs_pos--;
			else {
				if(offset) offset--;
			}
			break;
		case KEY_DOWN:
			if(curs_pos<row-1) {
				curs_pos++;
			}else {
				if(var_cnt && (offset+curs_pos<var_cnt-1)) offset++;
			}
			break;
		case KEY_LEFT:
			current_menu = MENU_MAIN;
			break;
		case KEY_RIGHT:
			if(var_cnt) current_menu = MENU_EDIT_U16;
			u16var_text[0] = ' ';
			u16var_text[1] = ' ';
			u16var_text[2] = ' ';
			u16var_text[3] = ' ';
			u16var_text[4] = ' ';
			u16var_text[5] = 0;
			var_x_pos = 0;
			break;
	}
}

void diagn_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	//bt816_cmd_setfont2(2,MEM_L1FONT22,0);
	//bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	bt816_cmd_setfont2(4,MEM_FONT40,0);

	uint16_t x_pos = 200;
	uint16_t y_pos = 15;
	uint16_t width = 400;
	uint16_t height = 50;
	uint16_t distance = 15;

	bt816_cmd_fgcolor(COLOR_RGB(100, 0, 0));
	bt816_cmd_button(x_pos, y_pos, width, height, 4, 0, "DI 1");
	bt816_cmd_button(x_pos, y_pos + height + distance, width, height, 4, 0, "AI 2");
	bt816_cmd_button(x_pos, y_pos + (height + distance)*2, width, height, 4, 0, "DO 3");
	bt816_cmd_button(x_pos, y_pos + (height + distance)*3, width, height, 4, 0, "CLUSTER BITS 4");
	bt816_cmd_button(x_pos, y_pos + (height + distance)*4, width, height, 4, 0, "NET BITS 5");
	bt816_cmd_button(x_pos, y_pos + (height + distance)*5, width, height, 4, 0, "CLUSTER REGS 6");
	bt816_cmd_button(x_pos, y_pos + (height + distance)*6, width, height, 4, 0, "NET REGS 7");

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_1:
			current_menu = MENU_DI;
			break;
		case KEY_2:
			current_menu = MENU_AI;
			break;
		case KEY_3:
			current_menu = MENU_DO;
			break;
		case KEY_4:
			current_menu = MENU_CLUST_BITS;
			offset = 0;
			break;
		case KEY_5:
			current_menu = MENU_NET_BITS;
			break;
		case KEY_6:
			current_menu = MENU_CLUST_REGS;
			break;
		case KEY_7:
			current_menu = MENU_NET_REGS;
			break;
		case KEY_LEFT:
			current_menu = MENU_MAIN;
			break;
	}
}

void di_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	bt816_cmd_setfont2(2,MEM_FONT22,0);
	//bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	//bt816_cmd_setfont2(4,MEM_L1FONT40,0);


	uint16_t x_pos = 50;
	uint16_t y_pos = 30;
	uint16_t data_x_pos = 200;
	uint16_t step = 30;
	for(uint16_t i=0;i<DI_CNT;i++) {
		uint32_t val = i+1;
		bt816_cmd_text_var(x_pos, y_pos, 2, OPT_FORMAT, "DI%d:", 1, &val);
		if(plc_di_type[i]==DI_UNUSED) {
			bt816_cmd_text(data_x_pos, y_pos, 2, 0, message_not_used);
		}else {
			switch(plc_di_state[i]) {
				case DI_ON:
					bt816_cmd_text(data_x_pos,y_pos,2,0,message_on);
					break;
				case DI_OFF:
					bt816_cmd_text(data_x_pos,y_pos,2,0,message_off);
					break;
				case DI_FAULT:
					bt816_cmd_text(data_x_pos,y_pos,2,0,"FAULT");
					break;
			}
		}
		y_pos+=step;
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
	if(key==KEY_LEFT) {
		current_menu = MENU_DIAGN;
	}
}

void do_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	bt816_cmd_setfont2(2,MEM_FONT22,0);
	//bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	//bt816_cmd_setfont2(4,MEM_L1FONT40,0);


	uint16_t x_pos = 50;
	uint16_t y_pos = 30;
	uint16_t data_x_pos = 200;
	uint16_t step = 30;
	for(uint16_t i=0;i<DO_CNT;i++) {
		uint32_t val = i+1;
		bt816_cmd_text_var(x_pos, y_pos, 2, OPT_FORMAT, "DO%d:", 1, &val);
		switch(plc_do_state[i]) {
			case DO_ON:
				bt816_cmd_text(data_x_pos,y_pos,2,0,message_on);
				break;
			case DO_OFF:
				bt816_cmd_text(data_x_pos,y_pos,2,0,message_off);
				break;
			case DO_FAULT:
				bt816_cmd_text(data_x_pos,y_pos,2,0,"FAULT");
				break;
		}
		y_pos+=step;
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
	if(key==KEY_LEFT) {
		current_menu = MENU_DIAGN;
	}
}

void ai_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint16_t x_pos = 50;
	uint16_t y_pos = 30;
	uint16_t data_x_pos = 200;
	uint16_t data_measure_pos = 300;
	uint16_t step = 30;
	for(uint16_t i=0;i<AI_CNT;i++) {
		uint32_t val = i+1;
		bt816_cmd_text_var(x_pos, y_pos, 2, OPT_FORMAT, "AI%d:", 1, &val);
		switch(plc_ai_type[i]) {
			case AI_UNUSED:
				bt816_cmd_text(data_x_pos, y_pos, 2, 0, message_not_used);
				break;
			case AI_U:
				bt816_cmd_number(data_x_pos, y_pos, 22, 0, plc_ain_raw[i]);
				bt816_cmd_text(data_measure_pos,y_pos,22,0,"");
				break;
			case AI_I:
				bt816_cmd_number(data_x_pos, y_pos, 22, 0, plc_ain_raw[i]);
				bt816_cmd_text(data_measure_pos,y_pos,22,0,"");
				break;
		}
		y_pos+=step;
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
	if(key==KEY_LEFT) {
		current_menu = MENU_DIAGN;
	}
}

void clust_bits_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint16_t x = 30;
	uint16_t y = 30;
	uint16_t x_step = 90;
	uint16_t y_step = 30;
	uint16_t col = 8;
	uint16_t row = 14;

	for(uint16_t i=0;i<row;i++) {
		for(uint16_t j=0;j<col;j++) {
			uint16_t num = i*col+j+offset;
			if(num<CLUST_BITS_CNT) {
				num+=17;
				uint32_t data[2] = {num,plc_clust_bits[num-17]};
				bt816_cmd_text_var(x+x_step*j, y+y_step*i, 22, OPT_FORMAT, "CB%d: %d", 2, data);
			}
		}
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_UP:
			if(offset>col) offset-=col;else offset = 0;
			break;
		case KEY_DOWN:
			offset+=col;
			if(offset>=CLUST_BITS_CNT-col) offset = CLUST_BITS_CNT-col;
			break;
		case KEY_LEFT:

			current_menu = MENU_DIAGN;
			break;
	}
}

void net_bits_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint16_t x = 30;
	uint16_t y = 30;
	uint16_t x_step = 90;
	uint16_t y_step = 30;
	uint16_t col = 8;
	uint16_t row = 14;

	for(uint16_t i=0;i<row;i++) {
		for(uint16_t j=0;j<col;j++) {
			uint16_t num = i*col+j+offset;
			if(num<NET_BITS_CNT) {
				num+=1;
				uint32_t data[2] = {num,plc_net_bits[num-1]};
				bt816_cmd_text_var(x+x_step*j, y+y_step*i, 22, OPT_FORMAT, "NB%d: %d", 2, data);
			}
		}
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key==KEY_LEFT) {
		current_menu = MENU_DIAGN;
	}
}

void clust_regs_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint16_t x = 30;
	uint16_t y = 30;
	uint16_t x_step = 150;
	uint16_t y_step = 30;
	uint16_t col = 5;
	uint16_t row = 14;

	for(uint16_t i=0;i<row;i++) {
		for(uint16_t j=0;j<col;j++) {
			uint16_t num = i*col+j+offset;
			if(num<CLUSTER_REGS_CNT) {
				num+=17;
				uint32_t data[2] = {num,plc_clust_regs[num-17]};
				bt816_cmd_text_var(x+x_step*j, y+y_step*i, 22, OPT_FORMAT, "CR%d: %d", 2, data);
			}
		}
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key==KEY_LEFT) {
		current_menu = MENU_DIAGN;
	}
}

void net_regs_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint16_t x = 30;
	uint16_t y = 30;
	uint16_t x_step = 150;
	uint16_t y_step = 30;
	uint16_t col = 5;
	uint16_t row = 14;

	for(uint16_t i=0;i<row;i++) {
		for(uint16_t j=0;j<col;j++) {
			uint16_t num = i*col+j+offset;
			if(num<NET_REGS_CNT) {
				num+=1;
				uint32_t data[2] = {num,plc_net_regs[num-1]};
				bt816_cmd_text_var(x+x_step*j, y+y_step*i, 22, OPT_FORMAT, "NR%d: %d", 2, data);
			}
		}
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key==KEY_LEFT) {
		current_menu = MENU_DIAGN;
	}
}
