/*
 * edit_var_menu.c
 *
 *  Created on: 2023 May 26
 *      Author: pelev
 */

#include "edit_var_menu.h"
#include <string.h>
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "cluster_state_menu.h"

static manage_var edit_var;
static uint8_t u16var_text[6]="     ";
static uint16_t var_x_pos = 0;

extern menu_list_t current_menu;

extern void write_var_to_plc(manage_var *var);

void set_edit_var(manage_var var) {
	edit_var = var;
	var_x_pos = 0;
	u16var_text[0] = ' ';
	u16var_text[1] = ' ';
	u16var_text[2] = ' ';
	u16var_text[3] = ' ';
	u16var_text[4] = ' ';
	u16var_text[5] = 0;
}

void edit_var_menu(uint16_t key) {
	uint32_t val = 0;

	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1C0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	//bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	//bt816_cmd_setfont2(2,MEM_L1FONT22,0);
	bt816_cmd_setfont2(3,MEM_FONT30,0);
	bt816_cmd_setfont2(4,MEM_FONT40,0);

	bt816_cmd_text(50,80,3,0,"\xd0\x92\xd0\xb2\xd0\xb5\xd0\xb4\xd0\xb8\xd1\x82\xd0\xb5\x20\xd0\xb7\xd0\xbd\xd0\xb0\xd1\x87\xd0\xb5\xd0\xbd\xd0\xb8\xd0\xb5");
	bt816_cmd_text(50,120,3,0,"\x45\x4e\x54\x45\x52\x20\x2d\x20\xd1\x81\xd0\xbe\xd1\x85\xd1\x80\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x82\xd1\x8c");
	bt816_cmd_text(50,160,3,0,"\x45\x53\x43\x20\x2d\x20\xd1\x81\xd0\xb1\xd1\x80\xd0\xbe\xd1\x81");
	bt816_cmd_text(50,200,3,0,"\xd0\xb2\xd0\xbb\xd0\xb5\xd0\xb2\xd0\xbe\x20\x2d\x20\xd0\xb2\xd0\xb5\xd1\x80\xd0\xbd\xd1\x83\xd1\x82\xd1\x8c\xd1\x81\xd1\x8f");

	bt816_cmd_text(50,350,3,0,edit_var.name);



	bt816_cmd_button(50, 270, 200, 50, 4, 0, u16var_text);

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
			current_menu = MENU_SYS_CFG;
			break;
		case KEY_EXIT:
			u16var_text[0] = ' ';
			u16var_text[1] = ' ';
			u16var_text[2] = ' ';
			u16var_text[3] = ' ';
			u16var_text[4] = ' ';
			u16var_text[5] = 0;
			var_x_pos = 0;
			break;
		case KEY_ENTER:
			write_var_to_plc(&edit_var);
			current_menu = MENU_SYS_CFG;
			break;
	}
}
