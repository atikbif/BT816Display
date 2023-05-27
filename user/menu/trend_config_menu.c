/*
 * trend_config.c
 *
 *  Created on: 2023 May 27
 *      Author: pelev
 */

#include "trend_config_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "config.h"
#include <string.h>
#include "add_trend_menu.h"
#include "trend_info_menu.h"
#include "delete_trend_menu.h"
#include "edit_trend_menu.h"

extern menu_list_t current_menu;

void trend_config_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1E0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_1:
			init_trend_info_menu();
			current_menu = MENU_TREND_INFO;
			break;
		case KEY_2:
			init_add_trend_menu();
			current_menu = MENU_ADD_TREND;
			break;
		case KEY_3:
			init_edit_trend_menu();
			current_menu = MENU_EDIT_TREND;
			break;
		case KEY_4:
			init_delete_trend_info_menu();
			current_menu = MENU_DELETE_TREND;
			break;
		case KEY_LEFT:
			current_menu = MENU_CONFIG;
			break;
	}
}
