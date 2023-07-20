/*
 * ip_config_menu.c
 *
 *  Created on: 2023 Jul 20
 *      Author: pelev
 */

#include "ip_config_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "ip_set_menu.h"

extern menu_list_t current_menu;

extern uint8_t rst_flag;
extern uint16_t rst_tmr;

void ip_config_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1C0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);


	bt816_cmd_setfont2(1,MEM_FONT30,0);

	bt816_cmd_text(60, 90, 1, 0, "\x49\x50\x20\xd0\xb0\xd0\xb4\xd1\x80\xd0\xb5\xd1\x81\x20\x20\x20\x20\x20\x31");
	bt816_cmd_text(60, 130, 1, 0, "\x49\x50\x20\xd0\xbc\xd0\xb0\xd1\x81\xd0\xba\xd0\xb0\x20\x20\x20\x20\x20\x32");
	bt816_cmd_text(60, 170, 1, 0, "\x49\x50\x20\xd1\x88\xd0\xbb\xd1\x8e\xd0\xb7\x20\x20\x20\x20\x20\x20\x33");
	bt816_cmd_text(60, 210, 1, 0, "\xd0\x9f\xd0\xb5\xd1\x80\xd0\xb5\xd0\xb7\xd0\xb0\xd0\xbf\xd1\x83\xd1\x81\xd0\xba\x20\x20\x20\x34");

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_LEFT:
			current_menu = MENU_CONFIG;
			break;
		case KEY_1:
			set_ip_type(IP_TYPE_ADDR);
			current_menu = MENU_IP_SET;
			break;
		case KEY_2:
			set_ip_type(IP_TYPE_MASK);
			current_menu = MENU_IP_SET;
			break;
		case KEY_3:
			set_ip_type(IP_TYPE_GATE);
			current_menu = MENU_IP_SET;
			break;
		case KEY_4:
			rst_flag = 1;
			rst_tmr = 1000;
			break;
	}
}
