/*
 * system_config_menu.c
 *
 *  Created on: 2023 May 26
 *      Author: pelev
 */

#include "system_config_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "config.h"
#include <string.h>
#include "edit_var_menu.h"

static uint16_t offset = 0;
static uint16_t curs_pos = 0;
static manage_var edit_var;

static uint8_t u16var_text[6]="     ";
static uint16_t var_x_pos = 0;

extern menu_list_t current_menu;

void system_confug_menu(uint16_t key) {

	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1C0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	bt816_cmd_setfont2(2,MEM_FONT22,0);
	//bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	bt816_cmd_setfont2(4,MEM_FONT40,0);

	uint16_t x_pos = 50;
	uint16_t y_pos = 96;
	uint16_t step = 30;
	uint16_t row = 10;
	uint16_t x_data_pos = 400;

	uint16_t var_cnt = 0;

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	for(uint16_t i=0;i<row;i++) {
		if(i==curs_pos) {
			bt816_cmd_fgcolor(COLOR_RGB(100, 100, 130));
			bt816_cmd_button(x_pos-10, y_pos+i*step, x_data_pos-x_pos, step+2, 2, 0, "");
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
		bt816_cmd_text(x_pos,y_pos,2,0,"NO DATA");
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
			current_menu = MENU_CONFIG;
			break;
		case KEY_RIGHT:
		case KEY_ENTER:
			set_edit_var(edit_var);
			if(var_cnt) current_menu = MENU_EDIT_VAR;
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
