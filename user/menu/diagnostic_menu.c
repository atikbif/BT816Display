/*
 * diagnostic_menu.c
 *
 *  Created on: 2023 May 12
 *      Author: pelev
 */

#include "diagnostic_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "cluster_state_menu.h"

extern menu_list_t current_menu;
extern cluster_info_data_type cluster_data;

static uint8_t prev_key=0;

void diagnostic_menu(uint16_t key) {

	uint16_t y_offset = 98;
	uint16_t step = 35;

	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x160000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(2,MEM_FONT22,0);

	for(uint8_t i=0;i<8;i++) {
		if(cluster_data.used_plc[i]) {

			switch(cluster_data.plc_type[i]) {
				case PC21_1:
					bt816_cmd_text(100, y_offset+i*step, 2, 0,"PC21-1");
					break;
				case PC21_CD:
					bt816_cmd_text(100, y_offset+i*step, 2, 0,"PC21-CD");
					break;
				case PC21_MC:
					bt816_cmd_text(100, y_offset+i*step, 2, 0,"PC21-MC");
					break;
				case PC21_2T:
					bt816_cmd_text(100, y_offset+i*step, 2, 0,"PC21-2T");
					break;
			}

		}else {
			bt816_cmd_text(100, y_offset+i*step, 2, 0,"........");
		}
		bt816_cmd_number(75, y_offset+i*step, 2, 0, i);
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key!=prev_key) {
		switch(key) {
			case KEY_LEFT:
				current_menu = MENU_MAIN;
				break;
			case KEY_RIGHT:
				break;
			case KEY_0:
				if(cluster_data.used_plc[0]) {

				}
				break;
			case KEY_1:
				if(cluster_data.used_plc[1]) {

				}
				break;
			case KEY_2:
				if(cluster_data.used_plc[2]) {

				}
				break;
			case KEY_3:
				if(cluster_data.used_plc[3]) {

				}
				break;
			case KEY_4:
				if(cluster_data.used_plc[4]) {

				}
				break;
			case KEY_5:
				if(cluster_data.used_plc[5]) {

				}
				break;
			case KEY_6:
				if(cluster_data.used_plc[6]) {

				}
				break;
			case KEY_7:
				if(cluster_data.used_plc[7]) {

				}
				break;
		}
	}
	prev_key = key;

}
