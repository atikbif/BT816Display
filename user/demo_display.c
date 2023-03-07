/*
 * demo_display.c
 *
 *  Created on: 2023 Feb 8
 *      Author: pelev
 */

#include "demo_display.h"
#include "bt816_cmd.h"
#include "at32f435_437_board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ram_map.h"
#include "fonts.h"

/* memory-map defines */

const uint8_t str1[] = "\xd0\xa2\xd0\xb5\xd1\x81\xd1\x82\xd0\xbe\xd0\xb2\xd0\xb0\xd1\x8f\x20\xd1\x81\xd1\x82\xd1\x80\xd0\xbe\xd0\xba\xd0\xb0\x21";

void init_display_fonts(void) {

	bt816_cmd_memwrite(MEM_L1GLYPH14, get_glyph_size(CYR_FONT14), get_glyph_data(CYR_FONT14));
	bt816_cmd_memwrite(MEM_L1FONT14, get_xfont_size(CYR_FONT14), get_xfont_data(CYR_FONT14));
	bt816_cmd_memwrite(MEM_L1GLYPH22, get_glyph_size(CYR_FONT22), get_glyph_data(CYR_FONT22));
	bt816_cmd_memwrite(MEM_L1FONT22, get_xfont_size(CYR_FONT22), get_xfont_data(CYR_FONT22));
	bt816_cmd_memwrite(MEM_L1GLYPH30, get_glyph_size(CYR_FONT30), get_glyph_data(CYR_FONT30));
	bt816_cmd_memwrite(MEM_L1FONT30, get_xfont_size(CYR_FONT30), get_xfont_data(CYR_FONT30));
	bt816_cmd_memwrite(MEM_L1GLYPH40, get_glyph_size(CYR_FONT40), get_glyph_data(CYR_FONT40));
	bt816_cmd_memwrite(MEM_L1FONT40, get_xfont_size(CYR_FONT40), get_xfont_data(CYR_FONT40));
}

void demo_display1(void) {
	uint16_t offset = 0;
	bt816_mem_write32(RAM_DL+offset, bt816_clear_color_rgb(0,0,0));offset+=4;
	bt816_mem_write32(RAM_DL+offset, bt816_clear(1,1,1));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_begin(BT816_BITMAPS));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2ii(215, 110, 31, 'T'));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2ii(244, 110, 31, 'E'));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2ii(270, 110, 31, 'S'));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2ii(299, 110, 31, 'T'));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_end());offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_color_rgb(160, 22, 22));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_point_size(320));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_begin(BT816_POINTS));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2ii(20, 20, 0, 0));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2ii(20, 460, 0, 0));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2f((800-20)*16, 20*16));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_vertex2f((800-20)*16, 460*16));offset+=4;
	bt816_mem_write32(RAM_DL+offset,bt816_end());offset+=4;
	bt816_mem_write32(RAM_DL+offset, bt816_display());offset+=4;
	bt816_mem_write32(REG_DLSWAP, DLSWAP_FRAME);
}

void demo_display_fonts(void) {
	bt816_cmd_dl(CMD_DLSTART);
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | 0x03);

	bt816_cmd_dl(DL_COLOR_RGB | GREEN);
	//bt816_cmd_dl(DL_END);

	bt816_cmd_setfont2(1,MEM_L1FONT14,0);
	bt816_cmd_setfont2(2,MEM_L1FONT22,0);
	bt816_cmd_setfont2(3,MEM_L1FONT30,0);
	bt816_cmd_setfont2(4,MEM_L1FONT40,0);

	uint8_t font1 = 1;
	uint8_t font2 = 2;
	uint8_t font3 = 3;
	uint8_t font4 = 4;

	uint8_t font1_step = 20;
	uint8_t font2_step = 25;
	uint8_t font3_step = 35;
	uint8_t font4_step = 45;

	uint32_t y_pos = 10;
	for(int i=0;i<3;i++) {
		bt816_cmd_text(50, y_pos, font1, 0, str1);y_pos+=font1_step;
		bt816_cmd_text(50, y_pos, font2, 0, str1);y_pos+=font2_step;
		bt816_cmd_text(50, y_pos, font3, 0, str1);y_pos+=font3_step;
		bt816_cmd_text(50, y_pos, font4, 0, str1);y_pos+=font4_step;
	}

	uint32_t v1 = bt816_mem_read32(REG_CMD_DL);
	uint32_t v2 = bt816_mem_read32(REG_CMDB_SPACE);


	v1 = bt816_mem_read32(REG_CMD_DL);
	v2 = bt816_mem_read32(REG_CMDB_SPACE);
	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
}

void display_value(uint16_t value) {
	bt816_cmd_dl(CMD_DLSTART);
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	bt816_cmd_dl(DL_COLOR_RGB | WHITE);
	bt816_cmd_dl(DL_END);

	bt816_cmd_number(100, 100, 31, BT816_OPT_CENTERX, value);

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
}
