/*
 * ip_set_menu.c
 *
 *  Created on: 2023 Jul 20
 *      Author: pelev
 */

#include "ip_set_menu.h"
#include "bt816_cmd.h"
#include "fonts.h"
#include "ram_map.h"
#include "keys.h"
#include "menu_list.h"
#include "ip_config_menu.h"
#include "config.h"

static uint8_t ip_data[4];
static uint8_t curs_pos = 0;
static uint8_t num_pos = 0;
static uint8_t ip_type = IP_TYPE_ADDR;

extern menu_list_t current_menu;

static void cursor_to_right() {
	curs_pos++;
	if(curs_pos>=3) {
		curs_pos = 0;
		num_pos++;
		if(num_pos>=4) {
			num_pos = 3;
			curs_pos = 2;
		}
	}
}

static void cursor_to_left() {
	if(curs_pos) curs_pos--;
	else {
		if(num_pos) {
			num_pos--;
			curs_pos = 2;
		}
	}
}

static void update_value(uint8_t num) {
	if(num<=9) {
		uint8_t *ptr = 0;
		if(num_pos<4) ptr = &ip_data[num_pos];
		if(ptr) {
			uint8_t value = *ptr;
			uint8_t a1 = value/100;
			uint8_t a2 = (value%100)/10;
			uint8_t a3 = value%10;
			if(curs_pos==0) a1 = num;
			else if(curs_pos==1) a2=num;
			else if(curs_pos==2) a3=num;

			*ptr = a1*100 + a2*10 + a3;
		}
	}
}

void set_ip_type(uint8_t value) {
	curs_pos = 0;
	num_pos = 0;
	switch(value) {
		case IP_TYPE_ADDR:
			ip_type = IP_TYPE_ADDR;
			read_ip_addr(ip_data);
			break;
		case IP_TYPE_MASK:
			ip_type = IP_TYPE_MASK;
			read_ip_mask(ip_data);
			break;
		case IP_TYPE_GATE:
			ip_type = IP_TYPE_GATE;
			read_ip_gate(ip_data);
			break;
	}
}

void ip_set_menu(uint16_t key) {
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t addr = 0x1C0000;

	bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(VERTEX2F(0, 0));
	bt816_cmd_dl(DL_END);


	bt816_cmd_setfont2(1,MEM_FONT30,0);

	switch(ip_type) {
		case IP_TYPE_ADDR:
			bt816_cmd_text(330, 90, 1, 0, "\x49\x50\x20\xd0\xb0\xd0\xb4\xd1\x80\xd0\xb5\xd1\x81");
			break;
		case IP_TYPE_MASK:
			bt816_cmd_text(330, 90, 1, 0, "\x49\x50\x20\xd0\xbc\xd0\xb0\xd1\x81\xd0\xba\xd0\xb0");
			break;
		case IP_TYPE_GATE:
			bt816_cmd_text(330, 90, 1, 0, "\x49\x50\x20\xd1\x88\xd0\xbb\xd1\x8e\xd0\xb7");
			break;
	}

	uint8_t ip_buf[] = "000.000.000.000";

	ip_buf[0] = ip_data[0]/100+'0';
	ip_buf[1] = (ip_data[0]%100)/10+'0';
	ip_buf[2] = ip_data[0]%10+'0';

	ip_buf[4] = ip_data[1]/100+'0';
	ip_buf[5] = (ip_data[1]%100)/10+'0';
	ip_buf[6] = ip_data[1]%10+'0';

	ip_buf[8] = ip_data[2]/100+'0';
	ip_buf[9] = (ip_data[2]%100)/10+'0';
	ip_buf[10] = ip_data[2]%10+'0';

	ip_buf[12] = ip_data[3]/100+'0';
	ip_buf[13] = (ip_data[3]%100)/10+'0';
	ip_buf[14] = ip_data[3]%10+'0';

	uint16_t curs_width = 18;
	uint16_t curs_height = 40;


	uint16_t x_pos = 265;
	uint16_t y_pos = 200;
	x_pos += num_pos*curs_width*4;
	x_pos += curs_pos*curs_width;


	bt816_cmd_dl(DL_COLOR_RGB | 0x808080UL);
	bt816_cmd_dl(DL_END);
	bt816_cmd_dl(DL_BEGIN | BT816_RECTS);
	bt816_cmd_dl(VERTEX2F(x_pos*16, y_pos*16));
	bt816_cmd_dl(VERTEX2F((x_pos + curs_width)*16, (y_pos+curs_height)*16));
	bt816_cmd_dl(DL_END);

	bt816_cmd_dl(DL_COLOR_RGB | 0xFFFFFFUL);
	bt816_cmd_dl(DL_END);

	bt816_cmd_text(265, 200, 1, 0, ip_buf);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);

	switch(key) {
		case KEY_EXIT:
			current_menu = MENU_IP_CONFIG;
			break;
		case KEY_LEFT:
			cursor_to_left();
			break;
		case KEY_RIGHT:
			cursor_to_right();
			break;
		case KEY_0:
			update_value(0);
			cursor_to_right();
			break;
		case KEY_1:
			update_value(1);
			cursor_to_right();
			break;
		case KEY_2:
			update_value(2);
			cursor_to_right();
			break;
		case KEY_3:
			update_value(3);
			cursor_to_right();
			break;
		case KEY_4:
			update_value(4);
			cursor_to_right();
			break;
		case KEY_5:
			update_value(5);
			cursor_to_right();
			break;
		case KEY_6:
			update_value(6);
			cursor_to_right();
			break;
		case KEY_7:
			update_value(7);
			cursor_to_right();
			break;
		case KEY_8:
			update_value(8);
			cursor_to_right();
			break;
		case KEY_9:
			update_value(9);
			cursor_to_right();
			break;
		case KEY_ENTER:
			current_menu = MENU_IP_CONFIG;
			switch(ip_type) {
				case IP_TYPE_ADDR:
					set_ip_addr(ip_data);
					break;
				case IP_TYPE_MASK:
					set_ip_mask(ip_data);
					break;
				case IP_TYPE_GATE:
					set_ip_gate(ip_data);
					break;
			}
			break;
	}
}
