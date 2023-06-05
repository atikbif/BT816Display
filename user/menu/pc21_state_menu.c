/*
 * pc21_state_menu.c
 *
 *  Created on: 2023 May 22
 *      Author: pelev
 */

#include "pc21_state_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "cluster_state.h"
#include "plc_data.h"
#include "config.h"

static uint8_t pc21_addr = 0;
static uint8_t prev_key=0;

extern menu_list_t current_menu;
extern cluster cl;
extern uint8_t plc_can_link;

extern calc_config calc[MAX_CALC_CNT];

static char header[] = "\xd0\xa3\xd0\x97\xd0\x95\xd0\x9b\x20\x31\x20\x50\x43\x32\x31";
static const uint8_t num_offset = 9;

static void print_ai_value(uint16_t x, uint16_t y, uint8_t font, uint8_t num) {
	int32_t val[2] = {0,0};
	val[0] = calc[num].result;
	switch(calc[num].prec) {
		case PR0:
			bt816_cmd_text_var(x, x, font, OPT_FORMAT, "%d", 1, val);
			break;
		case PR1:
			val[1] = val[0]%10;
			val[0] = val[0]/10;
			bt816_cmd_text_var(x, y, font, OPT_FORMAT, "%d.%1d", 2, val);
			break;
		case PR2:
			val[1] = val[0]%100;
			val[0] = val[0]/100;
			bt816_cmd_text_var(x, y, font, OPT_FORMAT, "%d.%2d", 2, val);
			break;
		case PR3:
			val[1] = val[0]%1000;
			val[0] = val[0]/1000;
			bt816_cmd_text_var(x, y, font, OPT_FORMAT, "%d.%3d", 2, val);
			break;
	}
}

void set_pc21_addr(uint8_t addr) {
	pc21_addr = addr;
}

void pc21_state_menu(uint16_t key) {
	static uint8_t tmp_buf[30];
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x180000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(2,MEM_FONT22,0);

	header[num_offset] = 0x30 + pc21_addr;
	bt816_cmd_text(45, 37, 2, 0, header);

	bt816_cmd_text(45, 65, 2, 0, "ID:");
	bt816_cmd_number(85, 65, 2, 0, cl.pc21.app_id);

	bt816_cmd_text(450, 37, 2, 0, "ETHIP:");
	bt816_cmd_number(530, 37, 2, 0, cl.pc21.ethip_state);

	bt816_cmd_text(450, 65, 2, 0, "HEART:");
	bt816_cmd_number(530, 65, 2, 0, cl.pc21.heartbeat);

	if(plc_can_link) {
		bt816_cmd_dl(DL_COLOR_RGB | GREEN);
		bt816_cmd_dl(DL_END);
		bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
		bt816_cmd_dl(VERTEX2F(250*16, 40*16));
		bt816_cmd_dl(VERTEX2F(400*16, 75*16));
		bt816_cmd_dl(DL_END);

		bt816_cmd_dl(DL_COLOR_RGB | BLACK);
		bt816_cmd_dl(DL_END);

		bt816_cmd_text(260, 40, 2, 0, "\xd0\x92\x20\xd1\x81\xd0\xb5\xd1\x82\xd0\xb8");
	}else {
		bt816_cmd_dl(DL_COLOR_RGB | RED);
		bt816_cmd_dl(DL_END);
		bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
		bt816_cmd_dl(VERTEX2F(250*16, 40*16));
		bt816_cmd_dl(VERTEX2F(400*16, 75*16));
		bt816_cmd_dl(DL_END);

		bt816_cmd_dl(DL_COLOR_RGB | BLACK);
		bt816_cmd_dl(DL_END);

		bt816_cmd_text(260, 40, 2, 0, "\xd0\x9d\xd0\xb5\x20\xd0\xb2\x20\xd1\x81\xd0\xb5\xd1\x82\xd0\xb8");
	}

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	bt816_cmd_text(45, 105, 2, 0, "\xd0\xa1\xd0\xbe\xd1\x81\xd1\x82\xd0\xbe\xd1\x8f\xd0\xbd\xd0\xb8\xd0\xb5");
	bt816_cmd_text(45, 135, 2, 0, "\xd0\xa0\xd0\xb5\xd0\xbb\xd0\xb5");

	for(int i=0;i<PC21_INP_CNT;i++) {
		if(cl.pc21.ain[i].used) {
			if(cl.pc21.ain[i].fault) {
				bt816_cmd_text(200+i*15,105,2,0,"F");
			}else {
				bt816_cmd_text(200+i*15,105,2,0,"H");
			}
		}else if(cl.pc21.din[i].used) {
			if(cl.pc21.din[i].fault) {
				bt816_cmd_text(200+i*15,105,2,0,"f");
			}else {
				if(cl.pc21.din[i].state) {
					bt816_cmd_text(200+i*15,105,2,0,"1");
				}else {
					bt816_cmd_text(200+i*15,105,2,0,"0");
				}
			}
		}else {
			bt816_cmd_text(200+i*15,105,2,0,".");
		}
	}

	for(int i=0;i<PC21_OUT_CNT;i++) {
		if(cl.pc21.dout[i].fault) {
			bt816_cmd_text(200+i*15,135,2,0,"F");
		}else {
			if(cl.pc21.dout[i].state) {
				bt816_cmd_text(200+i*15,135,2,0,"1");
			}else {
				bt816_cmd_text(200+i*15,135,2,0,"0");
			}
		}
	}

	bt816_cmd_text(45, 160, 2, 0, "\xd0\x92\xd0\xb2\xd0\xbe\xd0\xb4\xd1\x8b");

	for(int i=0;i<PC21_INP_CNT;i++) {
		int x = 45 + (i%3)*230;
		int y = 200 + (i/3)*40;
		bt816_cmd_number(x, y, 2, 0, i+1);
		if(cl.pc21.din[i].used) {
			bt816_cmd_text(x+35,y,2,0,"\xd0\x94\xd0\xb8\xd1\x81\xd0\xba\xd1\x80\xd0\xb5\xd1\x82\x2e");
		}else if(cl.pc21.ain[i].used) {
			print_ai_value(x+35,y,2,i);
			if(get_ai_meas_unit(0,i,tmp_buf)) {
				bt816_cmd_text(x+135, y, 2, 0, tmp_buf);
			}
		}else {
			bt816_cmd_text(x+30,y,2,0,"...");
		}
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	if(key!=prev_key) {
		switch(key) {
		case KEY_LEFT:
			current_menu = MENU_DIAGNOSTIC;
			break;
		}
	}
	prev_key = key;
}
