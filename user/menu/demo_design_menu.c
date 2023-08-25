/*
 * demo_design_menu.c
 *
 *  Created on: 2023 Jul 27
 *      Author: pelev
 */

#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "cluster_state.h"
#include "config.h"
#include "demo_design_menu.h"

extern menu_list_t current_menu;
static uint32_t cnt = 0;
static uint8_t state = 0;


void demo_design_menu(uint16_t key) {
	cnt++;
	if(cnt>=20) {
		cnt = 0;
		state++;
		if(state>=7) state = 0;
	}

	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t mem_addr = 0x300000 + 0x010000*state;

	bt816_cmd_setbitmap(0x800000 | (mem_addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_LEFT:
		case KEY_ENTER:
		case KEY_EXIT:
			current_menu = MENU_MAIN;
			break;
	}
}
