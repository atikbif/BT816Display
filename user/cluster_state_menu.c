/*
 * cluster_state_menu.c
 *
 *  Created on: 2023 May 10
 *      Author: pelev
 */

#include "cluster_state_menu.h"

#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"

extern menu_list_t current_menu;
cluster_info_data_type cluster_data;

void cluster_state_menu(uint16_t key) {
	uint16_t y_offset = 45;
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x120000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	uint8_t full_cluster = 1;
	for(uint8_t i=0;i<8;i++) {
		if((cluster_data.used_plc[i])&&(cluster_data.plc_link[i]==0)) full_cluster=0;
	}

	if(full_cluster) {
		bt816_cmd_dl(DL_COLOR_RGB | GREEN);
		bt816_cmd_dl(DL_END);
	}else {
		bt816_cmd_dl(DL_COLOR_RGB | YELLOW);
		bt816_cmd_dl(DL_END);
	}

	bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
	bt816_cmd_dl(VERTEX2F(40*16, y_offset*16));
	bt816_cmd_dl(VERTEX2F(400*16, (y_offset+40)*16));
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(3,MEM_FONT30,0);

	bt816_cmd_dl(DL_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_END);

	bt816_cmd_text(43, y_offset, 3, 0, "\xd0\x9a\xd0\x9b\xd0\x90\xd0\xa1\xd0\xa2\xd0\x95\xd0\xa0");

	if(full_cluster) {
		bt816_cmd_text(220, y_offset, 3, 0, "\xd0\x9f\xd0\xbe\xd0\xbb\xd0\xbd\xd1\x8b\xd0\xb9");
	}else {
		bt816_cmd_text(220, y_offset, 3, 0, "\xd0\x9d\xd0\xb5\xd0\xbf\xd0\xbe\xd0\xbb\xd0\xbd\xd1\x8b\xd0\xb9");
	}
	y_offset+=40;
	for(uint8_t i=0;i<8;i++) {
		if(cluster_data.used_plc[i]) {
			if(cluster_data.plc_link[i]) {
				bt816_cmd_dl(DL_COLOR_RGB | 0x00A60BUL);
				bt816_cmd_dl(DL_END);
				bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
				bt816_cmd_dl(VERTEX2F(40*16, (y_offset+i*40)*16));
				bt816_cmd_dl(VERTEX2F(400*16, (y_offset+40+i*40)*16));
				bt816_cmd_dl(DL_END);
				bt816_cmd_dl(DL_COLOR_RGB | WHITE);
				bt816_cmd_dl(DL_END);
				switch(cluster_data.plc_type[i]) {
					case PC21_1:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-1");
						break;
					case PC21_CD:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-CD");
						break;
					case PC21_MC:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-MC");
						break;
					case PC21_2T:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-2T");
						break;
				}
				bt816_cmd_text(220, y_offset+i*40, 3, 0,"\xd0\xb2\x20\xd1\x81\xd0\xb5\xd1\x82\xd0\xb8");
			}else {
				bt816_cmd_dl(DL_COLOR_RGB | RED);
				bt816_cmd_dl(DL_END);
				bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
				bt816_cmd_dl(VERTEX2F(40*16, (y_offset+i*40)*16));
				bt816_cmd_dl(VERTEX2F(400*16, (y_offset+40+i*40)*16));
				bt816_cmd_dl(DL_END);
				bt816_cmd_dl(DL_COLOR_RGB | WHITE);
				bt816_cmd_dl(DL_END);
				switch(cluster_data.plc_type[i]) {
					case PC21_1:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-1");
						break;
					case PC21_CD:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-CD");
						break;
					case PC21_MC:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-MC");
						break;
					case PC21_2T:
						bt816_cmd_text(70, y_offset+i*40, 3, 0,"PC21-2T");
						break;
				}
				bt816_cmd_text(220, y_offset+i*40, 3, 0,"\xd0\xbd\xd0\xb5\x20\xd0\xb2\x20\xd1\x81\xd0\xb5\xd1\x82\xd0\xb8");
			}

		}else {
			bt816_cmd_dl(DL_COLOR_RGB | BLUE);
			bt816_cmd_dl(DL_END);

			bt816_cmd_text(70, y_offset+i*40, 3, 0,"........");
		}
		bt816_cmd_number(43, y_offset+i*40, 3, 0, i);
	}

	if(full_cluster) {
		bt816_cmd_dl(DL_COLOR_RGB | GREEN);
		bt816_cmd_dl(DL_END);
	}else {
		bt816_cmd_dl(DL_COLOR_RGB | YELLOW);
		bt816_cmd_dl(DL_END);
	}

	y_offset = 45;

	bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
	bt816_cmd_dl(VERTEX2F(440*16, (y_offset)*16));
	bt816_cmd_dl(VERTEX2F(760*16, (y_offset+120)*16));

	bt816_cmd_dl(DL_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_END);
	bt816_cmd_text(443, y_offset, 3, 0,"\xd0\xa1\xd1\x82\xd0\xb0\xd1\x82\xd1\x83\xd1\x81\x20\xd0\x9a\xd0\xbb\xd0\xb0\xd1\x81\xd1\x82\xd0\xb5\xd1\x80\xd0\xb0");
	bt816_cmd_text(443, y_offset+40, 3, 0,"0      7");
	uint8_t step = 18;
	for(uint8_t i=0;i<8;i++) {
		if(cluster_data.plc_link[i]) bt816_cmd_text(443+i*step,y_offset+80,3,0,"1");
		else bt816_cmd_text(443+i*step,y_offset+80,3,0,"0");
	}

	bt816_cmd_dl(DL_COLOR_RGB | BLUE);
	bt816_cmd_dl(DL_END);
	bt816_cmd_text(260, 430, 3, 0,"\xd0\x94\xd0\xb5\xd0\xb2\xd0\xb8\xd1\x81\x20\xd0\x94\xd0\xb5\xd1\x80\xd0\xb1\xd0\xb8\x20\xd0\xa1\xd0\xb8\xd0\xb1\xd0\xb8\xd1\x80\xd1\x8c");

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key==KEY_LEFT) {
		current_menu = MENU_MAIN;
	}
}
