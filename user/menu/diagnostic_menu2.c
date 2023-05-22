/*
 * diagnostic_menu2.c
 *
 *  Created on: 2023 May 22
 *      Author: pelev
 */

#include "diagnostic_menu2.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "cluster_state_menu.h"

static uint8_t prev_key=0;
extern menu_list_t current_menu;

void diagnostic_menu2(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x170000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key!=prev_key) {
		switch(key) {
			case KEY_LEFT:
				current_menu = MENU_DIAGNOSTIC;
				break;
			case KEY_1:

				break;
			case KEY_2:
				break;
			case KEY_3:
				break;
			case KEY_4:
				break;
		}
	}
	prev_key = key;
}
