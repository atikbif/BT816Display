/*
 * appl_info_menu.c
 *
 *  Created on: 2023 May 10
 *      Author: pelev
 */

#include "appl_info_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"

extern menu_list_t current_menu;

appl_info_data_type appl_info_data;

void appl_info_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x110000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(2,MEM_FONT22,0);

	bt816_cmd_number(440, 117, 2, 0, appl_info_data.id);
	bt816_cmd_text(440, 149, 2, 0, appl_info_data.name);

	uint32_t vers[3];
	vers[0] = appl_info_data.vers_major;
	vers[1] = appl_info_data.vers_minor;
	bt816_cmd_text_var(440, 183, 2, OPT_FORMAT, "%d.%d", 2, vers);

	vers[0] = appl_info_data.date;
	vers[1] = appl_info_data.month;
	vers[2] = appl_info_data.year;
	bt816_cmd_text_var(440, 217, 2, OPT_FORMAT, "%d/%d/%d", 3, vers);

	bt816_cmd_text(440, 251, 2, 0, "IBM PLEX MONO");

	bt816_cmd_text(440, 251, 2, 0, "IBM PLEX MONO");

	bt816_cmd_text(440, 285, 2, 0, "\xd0\xa0\xd0\xa3\xd0\xa1\xd0\xa1\xd0\x9a\xd0\x98\xd0\x99");

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key==KEY_LEFT) {
		current_menu = MENU_MAIN;
	}
}
