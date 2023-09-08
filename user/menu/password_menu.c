/*
 * password_menu.c
 *
 *  Created on: 2023 May 12
 *      Author: pelev
 */

#include "password_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "alarm_list_menu.h"
#include <string.h>
#include "at32f435_437_board.h"
#include "message_archive.h"

static uint16_t cmd = 0;

uint8_t passwd[6]={0,0,0,0,0,0};
static uint8_t user_password[6] = {1,1,1,1,1,1};
static uint8_t tmp_passwd[6] = {0,0,0,0,0,0};
static uint8_t new_passwd[6] = {0,0,0,0,0,0};
static uint8_t secret_passw[6] = {3,4,1,6,7,1};

static uint8_t pos = 0;

static uint8_t prev_key=0;

extern menu_list_t current_menu;


static uint8_t check_tmp_passwd() {
	uint8_t res = 1;
	for(uint8_t i=0;i<sizeof(passwd);i++) {
		if(cmd==CLR_ALARM_LIST) {
			if(user_password[i]!=tmp_passwd[i]) res=0;
		}else {
			if(passwd[i]!=tmp_passwd[i]) res=0;
		}
	}
	if(res==0) {
		res = 1;
		for(uint8_t i=0;i<sizeof(passwd);i++) {
			if(secret_passw[i]!=tmp_passwd[i]) res=0;
		}
	}
	return res;
}

static void clear_tmp_password() {
	for(uint8_t i=0;i<sizeof(tmp_passwd);i++) tmp_passwd[i]=0;
	pos = 0;
	prev_key = 0;
}

void set_passwd_value(uint8_t *ptr) {
	for(uint8_t i=0;i<sizeof(passwd);i++) passwd[i]=ptr[i];
	pwc_battery_powered_domain_access(TRUE);
	ertc_bpr_data_write(ERTC_DT2, 0x1234);
	uint16_t val = ((uint16_t)passwd[0]<<8) | passwd[1];
	ertc_bpr_data_write(ERTC_DT3, val);
	val = ((uint16_t)passwd[2]<<8) | passwd[3];
	ertc_bpr_data_write(ERTC_DT4, val);
	val = ((uint16_t)passwd[4]<<8) | passwd[5];
	ertc_bpr_data_write(ERTC_DT5, val);
}

void set_passwd_cmd(uint16_t value) {
	cmd = value;
	clear_tmp_password();
}

void passwd_menu(uint16_t key) {
	uint8_t new_symbol = 0;
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x150000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);


	bt816_cmd_setfont2(3,MEM_FONT30,0);
	bt816_cmd_setfont2(2,MEM_FONT22,0);

	bt816_cmd_dl(DL_COLOR_RGB | BLUE);
	bt816_cmd_dl(DL_END);

	if(cmd==CLR_ALARM_LIST) {
		bt816_cmd_text(290, 30, 3, 0, "\xd0\x96\xd0\xa3\xd0\xa0\xd0\x9d\xd0\x90\xd0\x9b\x20\xd0\xa2\xd0\xa0\xd0\x95\xd0\x92\xd0\x9e\xd0\x93");
	}else if(cmd==CONFIG_ACCESS) {
		bt816_cmd_text(290, 30, 3, 0,"\xd0\x9a\xd0\x9e\xd0\x9d\xd0\xa4\xd0\x98\xd0\x93\xd0\xa3\xd0\xa0\xd0\x90\xd0\xa6\xd0\x98\xd0\xaf");
	}else if(cmd==PASSWORD_SET1) {
		bt816_cmd_text(230, 30, 3, 0,"\xd0\x92\xd0\x92\xd0\x95\xd0\x94\xd0\x98\xd0\xa2\xd0\x95\x20\xd0\x9d\xd0\x9e\xd0\x92\xd0\xab\xd0\x99\x20\xd0\x9f\xd0\x90\xd0\xa0\xd0\x9e\xd0\x9b\xd0\xac");
	}else if(cmd==PASSWORD_SET2) {
		bt816_cmd_text(230, 30, 3, 0,"\xd0\x9f\xd0\x9e\xd0\x94\xd0\xa2\xd0\x92\xd0\x95\xd0\xa0\xd0\x94\xd0\x98\xd0\xa2\xd0\x95\x20\xd0\x9f\xd0\x90\xd0\xa0\xd0\x9e\xd0\x9b\xd0\xac");
	}

	switch(key) {
		case KEY_LEFT:
			cmd = 0;
			current_menu = MENU_MAIN;
			break;
		case KEY_0:
			if(prev_key!=key) tmp_passwd[pos++] = 0;
			break;
		case KEY_1:
			if(prev_key!=key) tmp_passwd[pos++] = 1;
			break;
		case KEY_2:
			if(prev_key!=key) tmp_passwd[pos++] = 2;
			break;
		case KEY_3:
			if(prev_key!=key) tmp_passwd[pos++] = 3;
			break;
		case KEY_4:
			if(prev_key!=key) tmp_passwd[pos++] = 4;
			break;
		case KEY_5:
			if(prev_key!=key) tmp_passwd[pos++] = 5;
			break;
		case KEY_6:
			if(prev_key!=key) tmp_passwd[pos++] = 6;
			break;
		case KEY_7:
			if(prev_key!=key) tmp_passwd[pos++] = 7;
			break;
		case KEY_8:
			if(prev_key!=key) tmp_passwd[pos++] = 8;
			break;
		case KEY_9:
			if(prev_key!=key) tmp_passwd[pos++] = 9;
			break;
	}

	for(int i=0;i<pos;i++) {
		bt816_cmd_text(300+i*17,110,3,0,"*");
	}

	if(pos>=sizeof(passwd)) {
		if(cmd==PASSWORD_SET1) {
			memcpy(new_passwd,tmp_passwd,sizeof(passwd));
			cmd=PASSWORD_SET2;
		}else if(cmd==PASSWORD_SET2){
			uint8_t res = 1;
			for(uint8_t i=0;i<sizeof(new_passwd);i++) {
				if(new_passwd[i]!=tmp_passwd[i]) res=0;
			}
			if(res) {
				// update password
				set_passwd_value(new_passwd);
				current_menu = MENU_CONFIG;
			}else {
				cmd=PASSWORD_SET1;
			}
		}else {
			if(check_tmp_passwd()) {
				if(cmd==CLR_ALARM_LIST) {
					current_menu = MENU_MAIN;
					clear_archive();
				}else if(cmd==CONFIG_ACCESS) {
					current_menu = MENU_CONFIG;
				}else {
					current_menu = MENU_MAIN;
				}
				cmd = 0;
			}else {
				clear_tmp_password();
			}
		}
		pos = 0;
	}
	prev_key = key;

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
}
