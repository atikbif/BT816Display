/*
 * prog_menu.c
 *
 *  Created on: 2023 May 26
 *      Author: pelev
 */

#include "prog_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"


static uint8_t prog_menu_cmd = 0;
extern menu_list_t current_menu;

void init_prog_menu() {
	prog_menu_cmd = 1;
}

void prog_menu(uint16_t key) {
	if(prog_menu_cmd) {
		bt816_cmd_dl(CMD_DLSTART); /* start the display list */
		bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
		bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

		bt816_cmd_text(300,100,30,0,"PROG MODE");

		bt816_cmd_dl(DL_DISPLAY);
		bt816_cmd_dl(CMD_SWAP);
	}
	prog_menu_cmd = 0;
	switch(key) {
		case KEY_LEFT:
		case KEY_ENTER:
		case KEY_EXIT:
			current_menu = MENU_MAIN;
			break;
	}
}
