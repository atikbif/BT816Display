/*
 * config_menu.c
 *
 *  Created on: 2023 May 26
 *      Author: pelev
 */

#include "config_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "password_menu.h"
#include "set_time_menu.h"
#include "ip_config_menu.h"

extern menu_list_t current_menu;

void config_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1D0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);


	bt816_cmd_setfont2(1,MEM_FONT14,0);
	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_LEFT:
			current_menu = MENU_MAIN;
			break;
		case KEY_1:
			current_menu = MENU_SYS_CFG;
			break;
		case KEY_2:
			init_time_menu();
			current_menu = MENU_SET_TIME;
			break;
		case KEY_3:
			current_menu = MENU_TREND_CONF;
			break;
		case KEY_4:
			set_passwd_cmd(PASSWORD_SET1);
			current_menu = MENU_PASSWD;
			break;
		case KEY_5:
			current_menu = MENU_IP_CONFIG;
			break;
	}
}
