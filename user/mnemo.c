/*
 * mnemo.c
 *
 *  Created on: 2023 Feb 28
 *      Author: pelev
 */

#include "mnemo.h"
#include "bt816_cmd.h"
#include "demo_display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lamps.h"
#include "ram_map.h"
#include "plc_data.h"
#include "var_link.h"

uint16_t mnemo_cnt = 0;



#define MNEMO_DESCR_START			0x15A2
#define MNEMO_ID_LAMP 				1
#define MNEMO_ID_BACKGROUND_IMAGE 	2
#define MNEMO_ID_LOAD_CYR_FONTS		3
#define MNEMO_ID_TEXT				4
#define MNEMO_ID_BACKGROUND_COL 	5
#define MNEMO_ID_GAUGE				6
#define MNEMO_ID_TEXT_VAR			7
#define MNEMO_ID_NUMBER				8
#define MNEMO_ID_PROGR				9

#define MNEMO_HEADER_LENGTH			4
#define MNEMO_ONE_ELEM_LINK_WIDTH	2

extern enum DI_STATE plc_di_state[DI_CNT];
extern enum DO_STATE plc_do_state[DO_CNT];

extern uint16_t plc_ain_raw[AI_CNT];
extern uint8_t plc_clust_bits[CLUST_BITS_CNT];
extern uint16_t plc_clust_regs[CLUSTER_REGS_CNT];
extern uint8_t plc_net_bits[NET_BITS_CNT];
extern uint16_t plc_net_regs[NET_REGS_CNT];

extern calc_config calc[MAX_CALC_CNT];

extern uint8_t plc_can_link;

#define XPOS_MAX	800
#define YPOS_MAX	480

const uint8_t mnemo1_data[] = {
	0x15,0xA2,0x00,27,	// 0x00
	0x00,0x42,	// 0x04
	0x00,0x49,	// 0x06
	0x00,0x57,	// 0x08
	0x00,0x65,	// 0x0A
	0x00,0x73,	// 0x0C
	0x00,0x81,	// 0x0E
	0x00,0x8F,	// 0x10
	0x00,0x9D,	// 0x12
	0x00,0xAB,	// 0x14
	0x00,0xB9,	// 0x16
	0x00,0xC7,	// 0x18
	0x00,0xD5,	// 0x1A
	0x00,0xE3,	// 0x1C
	0x00,0xF1,	// 0x1E
	0x00,0xFF,	// 0x20
	0x01,0x0E,	// 0x22
	0x01,0x1D,	// 0x24
	0x01,0x2C,	// 0x26
	0x01,0x3B,	// 0x28
	0x01,0x4A,	// 0x2A
	0x01,0x59,	// 0x2C
	0x01,0x68,	// 0x2E
	0x01,0x77,	// 0x30
	0x01,0x86,	// 0x32
	0x01,0x95,	// 0x34
	0x01,0xA4,	// 0x36
	0x01,0xB3,	// 0x38
	0x00,0x00,	// 0x3A
	0x00,0x00,	// 0x3C
	0x00,0x00,	// 0x3E
	0x00,0x00,	// 0x40
	MNEMO_ID_BACKGROUND_IMAGE>>8,MNEMO_ID_BACKGROUND_IMAGE&0xFF,1,0x00,0x20,0x00,0x00,	// 0x42
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_GREEN,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DI>>8,VAR_LINK_DI&0xFF,0,0,0,75,0,50,	// 0x49
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_GREEN,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DI>>8,VAR_LINK_DI&0xFF,0,1,0,175,0,50,	// 0x57
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_GREEN,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DI>>8,VAR_LINK_DI&0xFF,0,2,275>>8,275&0xFF,0,50,	// 0x65
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_GREEN,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DI>>8,VAR_LINK_DI&0xFF,0,3,375>>8,375&0xFF,0,50,	// 0x73
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_GREEN,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DI>>8,VAR_LINK_DI&0xFF,0,4,475>>8,475&0xFF,0,50,	// 0x81
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_GREEN,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DI>>8,VAR_LINK_DI&0xFF,0,5,575>>8,575&0xFF,0,50,	// 0x8F
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_GREEN,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DI>>8,VAR_LINK_DI&0xFF,0,6,675>>8,675&0xFF,0,50,	// 0x9D

	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_RED,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DO>>8,VAR_LINK_DO&0xFF,0,0,0,125,0,120,	// 0xAB
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_RED,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DO>>8,VAR_LINK_DO&0xFF,0,1,0,225,0,120,	// 0xB9
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_RED,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DO>>8,VAR_LINK_DO&0xFF,0,2,325>>8,325&0xFF,0,120,	// 0xC7
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_RED,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DO>>8,VAR_LINK_DO&0xFF,0,3,425>>8,425&0xFF,0,120,	// 0xD5
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_RED,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DO>>8,VAR_LINK_DO&0xFF,0,4,525>>8,525&0xFF,0,120,	// 0xE3
	MNEMO_ID_LAMP>>8,MNEMO_ID_LAMP&0xFF,1,LAMP_RED,LAMP_GRAY,LAMP_YELLOW,VAR_LINK_DO>>8,VAR_LINK_DO&0xFF,0,5,625>>8,625&0xFF,0,120,	// 0xF1

	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,90>>8,90&0xFF,105>>8,105&0xFF,26,'D','I','1',0,	// 0xFF
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,190>>8,190&0xFF,105>>8,105&0xFF,26,'D','I','2',0,	// 0x10E
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,290>>8,290&0xFF,105>>8,105&0xFF,26,'D','I','3',0,	// 0x11D
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,390>>8,390&0xFF,105>>8,105&0xFF,26,'D','I','4',0,	// 0x12C
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,490>>8,490&0xFF,105>>8,105&0xFF,26,'D','I','5',0,	// 0x13B
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,590>>8,590&0xFF,105>>8,105&0xFF,26,'D','I','6',0,	// 0x14A
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,690>>8,690&0xFF,105>>8,105&0xFF,26,'D','I','7',0,	// 0x159

	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,140>>8,140&0xFF,175>>8,175&0xFF,26,'D','O','1',0,	// 0x168
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,240>>8,240&0xFF,175>>8,175&0xFF,26,'D','O','2',0,	// 0x177
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,340>>8,340&0xFF,175>>8,175&0xFF,26,'D','O','3',0,	// 0x186
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,440>>8,440&0xFF,175>>8,175&0xFF,26,'D','O','4',0,	// 0x195
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,540>>8,540&0xFF,175>>8,175&0xFF,26,'D','O','5',0,	// 0x1A4
	MNEMO_ID_TEXT>>8,MNEMO_ID_TEXT&0xFF,1,0,0,0,640>>8,640&0xFF,175>>8,175&0xFF,26,'D','O','6',0,	// 0x1B3
};

const uint8_t mnemo2_data[] = {
	0x15,0xA2,0x00,0x01,	// 0x00
	0x00,0x06,	// 0x04
	MNEMO_ID_BACKGROUND_IMAGE>>8,MNEMO_ID_BACKGROUND_IMAGE&0xFF,1,0x00,0x21,0x00,0x00,	// 0x06
};

uint8_t mnemo_buf[4096];

static void mnemo_draw_lamp_image(uint16_t x, uint16_t y, enum LAMP_COL col);

static void mnemo_load_cyr_fonts(const uint8_t *ptr);
static void mnemo_draw_background_image(const uint8_t *ptr);
static void mnemo_draw_lamp(const uint8_t *ptr);
static void mnemo_draw_text(const uint8_t *ptr);
static void mnemo_draw_background_col(const uint8_t *ptr);


uint8_t read_mnemo_data(uint16_t mnemo_num) {
	if(mnemo_num==0) {
		for(uint16_t i=0;i<sizeof(mnemo1_data);i++) mnemo_buf[i] = mnemo1_data[i];
		if((mnemo_buf[0]==MNEMO_DESCR_START>>8)&&(mnemo_buf[0]==MNEMO_DESCR_START&0xFF)) {
			// check crc
			return 1;
		}
	}else if(mnemo_num==1) {
		for(uint16_t i=0;i<sizeof(mnemo2_data);i++) mnemo_buf[i] = mnemo2_data[i];
		if((mnemo_buf[0]==MNEMO_DESCR_START>>8)&&(mnemo_buf[0]==MNEMO_DESCR_START&0xFF)) {
			// check crc
			return 1;
		}
	}
	return 0;
}

void mnemo_draw_background_col(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_BACKGROUND_COL) {
		uint8_t ver = ptr[2];
		uint8_t red_val = ptr[3];
		uint8_t green_val = ptr[4];
		uint8_t blue_val = ptr[5];
		uint32_t col = DL_CLEAR_COLOR_RGB;
		col |= (uint32_t)red_val<<16;
		col |= (uint32_t)green_val<<8;
		col |= blue_val;
		bt816_cmd_dl(col);
	}
}

void mnemo_draw_text(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_TEXT) {
		uint8_t ver = ptr[2];
		uint8_t red_val = ptr[3];
		uint8_t green_val = ptr[4];
		uint8_t blue_val = ptr[5];
		uint16_t x_pos = ((uint16_t)ptr[6]<<8)|ptr[7];
		uint16_t y_pos = ((uint16_t)ptr[8]<<8)|ptr[9];
		uint8_t font = ptr[10];
		const uint8_t *str = &ptr[11];
		if(x_pos>=XPOS_MAX)  return;
		if(y_pos>=YPOS_MAX)	return;
		uint32_t col = DL_COLOR_RGB;
		col |= (uint32_t)red_val<<16;
		col |= (uint32_t)green_val<<8;
		col |= blue_val;
		bt816_cmd_dl(col);
		bt816_cmd_text(x_pos, y_pos, font, 0, str);
	}
}

void mnemo_load_cyr_fonts(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_LOAD_CYR_FONTS) {
		uint8_t ver = ptr[2];
		bt816_cmd_setfont2(1,MEM_FONT14,32);
		bt816_cmd_setfont2(2,MEM_FONT22,32);
		bt816_cmd_setfont2(3,MEM_FONT30,32);
		bt816_cmd_setfont2(4,MEM_FONT40,32);
	}
}

void mnemo_draw_progress(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_PROGR) {
		uint8_t ver = ptr[2];
		uint8_t red_val_fg = ptr[3];
		uint8_t green_val_fg = ptr[4];
		uint8_t blue_val_fg = ptr[5];
		uint8_t red_val_bg = ptr[6];
		uint8_t green_val_bg = ptr[7];
		uint8_t blue_val_bg = ptr[8];
		uint16_t x_pos = ((uint16_t)ptr[9]<<8)|ptr[10];
		uint16_t y_pos = ((uint16_t)ptr[11]<<8)|ptr[12];
		if(x_pos>=XPOS_MAX)  return;
		if(y_pos>=YPOS_MAX)	return;
		uint16_t width = ((uint16_t)ptr[13]<<8)|ptr[14];
		uint16_t height = ((uint16_t)ptr[15]<<8)|ptr[16];
		uint16_t max_value = ((uint16_t)ptr[17]<<8)|ptr[18];
		uint16_t link_type = ((uint16_t)ptr[19]<<8)|ptr[20];
		uint16_t link_index = ((uint16_t)ptr[21]<<8)|ptr[22];
		uint16_t value;
		switch(link_type) {
			case VAR_LINK_CL_REG:
				if(link_index<CLUSTER_REGS_CNT) value = plc_clust_regs[link_index];
				break;
			case VAR_LINK_NET_REG:
				if(link_index<NET_REGS_CNT) value = plc_net_regs[link_index];
				break;
			case VAR_LINK_RAW_AI:
				if(link_index<AI_CNT) value = plc_ain_raw[link_index];
				break;
			case VAR_LINK_CALC:
				if(link_index<MAX_CALC_CNT) value = (uint16_t)calc[link_index].result;
				break;
		}
		if(value>max_value) value = max_value;
		if(height>width) {
			//value = max_value - value;
		}
		uint32_t col = red_val_fg;
		col = col << 8;col |= green_val_fg;
		col = col << 8;col |= blue_val_fg;
		//bt816_cmd_fgcolor(col);
		col = red_val_bg;
		col = col << 8;col |= green_val_bg;
		col = col << 8;col |= blue_val_bg;
		bt816_cmd_bgcolor(col);
		bt816_cmd_progress(x_pos, y_pos, width, height, 0, value, max_value);
	}
}

void mnemo_draw_text_var(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_TEXT_VAR) {
		uint8_t ver = ptr[2];
		uint16_t x_pos = ((uint16_t)ptr[3]<<8)|ptr[4];
		uint16_t y_pos = ((uint16_t)ptr[5]<<8)|ptr[6];
		if(x_pos>=XPOS_MAX)  return;
		if(y_pos>=YPOS_MAX)	return;
		uint8_t font  = ptr[7];
		uint8_t precision = ptr[8];
		if(precision>PR3) precision = PR3;
		uint16_t link_type = ((uint16_t)ptr[9]<<8)|ptr[10];
		uint16_t link_index = ((uint16_t)ptr[11]<<8)|ptr[12];
		uint8_t red_val = ptr[13];
		uint8_t green_val = ptr[14];
		uint8_t blue_val = ptr[15];
		int32_t value[2] = {0,0};
		switch(link_type) {
			case VAR_LINK_CL_REG:
				if(link_index<CLUSTER_REGS_CNT) value[0] = plc_clust_regs[link_index];
				break;
			case VAR_LINK_NET_REG:
				if(link_index<NET_REGS_CNT) value[0] = plc_net_regs[link_index];
				break;
			case VAR_LINK_RAW_AI:
				if(link_index<AI_CNT) value[0] = plc_ain_raw[link_index];
				break;
			case VAR_LINK_CALC:
				if(link_index<MAX_CALC_CNT) value[0] = calc[link_index].result;
				break;
		}
		uint32_t col = DL_COLOR_RGB;
		col |= (uint32_t)red_val<<16;
		col |= (uint32_t)green_val<<8;
		col |= blue_val;
		bt816_cmd_dl(col);
		switch(precision) {
			case PR0:
				bt816_cmd_text_var(x_pos, y_pos, font, OPT_FORMAT, "%d", 1, value);
				break;
			case PR1:
				value[1] = value[0]%10;
				value[0] = value[0]/10;
				bt816_cmd_text_var(x_pos, y_pos, font, OPT_FORMAT, "%d.%1d", 2, value);
				break;
			case PR2:
				value[1] = value[0]%100;
				value[0] = value[0]/100;
				bt816_cmd_text_var(x_pos, y_pos, font, OPT_FORMAT, "%d.%2d", 2, value);
				break;
			case PR3:
				value[1] = value[0]%1000;
				value[0] = value[0]/1000;
				bt816_cmd_text_var(x_pos, y_pos, font, OPT_FORMAT, "%d.%3d", 2, value);
				break;
		}
	}
}

void mnemo_draw_number(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_TEXT_VAR) {
		uint8_t ver = ptr[2];
		uint16_t x_pos = ((uint16_t)ptr[3]<<8)|ptr[4];
		uint16_t y_pos = ((uint16_t)ptr[5]<<8)|ptr[6];
		if(x_pos>=XPOS_MAX)  return;
		if(y_pos>=YPOS_MAX)	return;
		uint8_t font  = ptr[7];
		uint8_t sign_on = ptr[8];
		uint16_t link_type = ((uint16_t)ptr[9]<<8)|ptr[10];
		uint16_t link_index = ((uint16_t)ptr[11]<<8)|ptr[12];
		uint8_t red_val = ptr[13];
		uint8_t green_val = ptr[14];
		uint8_t blue_val = ptr[15];
		int32_t value = 0;
		switch(link_type) {
			case VAR_LINK_CL_REG:
				if(link_index<CLUSTER_REGS_CNT) value = plc_clust_regs[link_index];
				break;
			case VAR_LINK_NET_REG:
				if(link_index<NET_REGS_CNT) value = plc_net_regs[link_index];
				break;
			case VAR_LINK_RAW_AI:
				if(link_index<AI_CNT) value = plc_ain_raw[link_index];
				break;
			case VAR_LINK_CALC:
				if(link_index<MAX_CALC_CNT) value = calc[link_index].result;
				break;
		}
		uint32_t col = DL_COLOR_RGB;
		col |= (uint32_t)red_val<<16;
		col |= (uint32_t)green_val<<8;
		col |= blue_val;
		bt816_cmd_dl(col);
		if(sign_on) {
			bt816_cmd_number(x_pos, y_pos, font, OPT_SIGNED, value);
		}else {
			bt816_cmd_number(x_pos, y_pos, font, 0, value);
		}
	}
}

void mnemo_draw_gauge(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_GAUGE) {
		uint8_t ver = ptr[2];
		uint16_t x_pos = ((uint16_t)ptr[3]<<8)|ptr[4];
		uint16_t y_pos = ((uint16_t)ptr[5]<<8)|ptr[6];
		if(x_pos>=XPOS_MAX)  return;
		if(y_pos>=YPOS_MAX)	return;
		uint16_t rad = ((uint16_t)ptr[7]<<8)|ptr[8];
		uint8_t major = ptr[9];
		uint8_t minor = ptr[10];
		uint16_t max_value = ((uint16_t)ptr[11]<<8)|ptr[12];
		uint16_t link_type = ((uint16_t)ptr[13]<<8)|ptr[14];
		uint16_t link_index = ((uint16_t)ptr[15]<<8)|ptr[16];
		uint8_t red_color = ptr[17];
		uint8_t green_color = ptr[18];
		uint8_t blue_color = ptr[19];
		uint16_t min_alarm = ((uint16_t)ptr[20]<<8)|ptr[21];
		uint16_t max_alarm = ((uint16_t)ptr[22]<<8)|ptr[23];
		uint16_t value = 0;
		switch(link_type) {
		case VAR_LINK_CL_REG:
			if(link_index<CLUSTER_REGS_CNT) value = plc_clust_regs[link_index];
			break;
		case VAR_LINK_NET_REG:
			if(link_index<NET_REGS_CNT) value = plc_net_regs[link_index];
			break;
		case VAR_LINK_RAW_AI:
			if(link_index<AI_CNT) value = plc_ain_raw[link_index];
			break;
		case VAR_LINK_CALC:
			if(link_index<MAX_CALC_CNT) value = (uint16_t)calc[link_index].result;
			break;
		}
		if(value>max_value) value = max_value;
		uint32_t color = 0;
		if((value<min_alarm) || (value>max_alarm)) {
			color = 0xFF0000;
		}else {
			color = red_color;
			color = color<<8;color |= green_color;
			color = color<<8;color |= blue_color;
		}
		bt816_cmd_bgcolor(color);
		bt816_cmd_gauge(x_pos, y_pos, rad, 0, major, minor, value, max_value);
	}
}

void mnemo_draw_background_image(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_BACKGROUND_IMAGE) {
		uint8_t version = ptr[2];
		uint32_t addr = ptr[3];
		addr = addr<<8;addr|=ptr[4];
		addr = addr<<8;addr|=ptr[5];
		addr = addr<<8;addr|=ptr[6];

		bt816_cmd_setbitmap(0x800000 | (addr/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
		bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
		bt816_cmd_dl(VERTEX2F(0, 0));
		bt816_cmd_dl(DL_END);
	}
}

void mnemo_draw_lamp(const uint8_t *ptr) {
	uint16_t id = ((uint16_t)ptr[0]<<8)|ptr[1];
	if(id==MNEMO_ID_LAMP) {
		uint8_t version = ptr[2];
		uint8_t on_color = ptr[3];
		uint8_t off_color = ptr[4];
		uint8_t fault_color = ptr[5];
		uint16_t link_type = ((uint16_t)ptr[6]<<8)|ptr[7];
		uint16_t link_index = ((uint16_t)ptr[8]<<8)|ptr[9];
		uint16_t x_pos = ((uint16_t)ptr[10]<<8)|ptr[11];
		uint16_t y_pos = ((uint16_t)ptr[12]<<8)|ptr[13];
		if(x_pos>=XPOS_MAX)  return;
		if(y_pos>=YPOS_MAX)	return;
		switch(link_type) {
			case VAR_LINK_DI:
				if(link_index<DI_CNT) {
					if(plc_di_state[link_index]==DI_ON) mnemo_draw_lamp_image(x_pos,y_pos,on_color);
					else if(plc_di_state[link_index]==DI_OFF) mnemo_draw_lamp_image(x_pos,y_pos,off_color);
					else if(plc_di_state[link_index]==DI_FAULT) mnemo_draw_lamp_image(x_pos,y_pos,fault_color);
				}
				break;
			case VAR_LINK_DO:
				if(link_index<DO_CNT) {
					if(plc_do_state[link_index]==DO_ON) mnemo_draw_lamp_image(x_pos,y_pos,on_color);
					else if(plc_do_state[link_index]==DO_OFF) mnemo_draw_lamp_image(x_pos,y_pos,off_color);
					else if(plc_do_state[link_index]==DO_FAULT) mnemo_draw_lamp_image(x_pos,y_pos,fault_color);
				}
				break;
		}
	}
}

uint16_t mnemo_read_cnt_from_config(uint32_t conf_addr) {
	return mnemo_cnt;
}

static uint16_t mnemo_get_elements_cnt() {
	uint16_t res = ((uint16_t)mnemo_buf[2]<<8)|mnemo_buf[3];
	return res;
}

void mnemo_draw_lamp_image(uint16_t x, uint16_t y, enum LAMP_COL col) {
	uint8_t correct = 0;
	switch(col) {
		case LAMP_GRAY:
			bt816_cmd_setbitmap(MEM_LAMP1, BT816_ARGB4, 50, 50);
			correct = 1;
			break;
		case LAMP_RED:
			bt816_cmd_setbitmap(MEM_LAMP2, BT816_ARGB4, 50, 50);
			correct = 1;
			break;
		case LAMP_BLUE:
			bt816_cmd_setbitmap(MEM_LAMP3, BT816_ARGB4, 50, 50);
			correct = 1;
			break;
		case LAMP_YELLOW:
			bt816_cmd_setbitmap(MEM_LAMP4, BT816_ARGB4, 50, 50);
			correct = 1;
			break;
		case LAMP_GREEN:
			bt816_cmd_setbitmap(MEM_LAMP5, BT816_ARGB4, 50, 50);
			correct = 1;
			break;
	}
	bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
	bt816_cmd_dl(bt816_vertex2f(x*16, y*16));
	bt816_cmd_dl(DL_END);
}

void mnemo_draw_element(uint16_t conf_addr) {
	uint16_t id = ((uint16_t)mnemo_buf[conf_addr]<<8)|mnemo_buf[conf_addr+1];
	switch(id) {
		case MNEMO_ID_LAMP:
			mnemo_draw_lamp(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_BACKGROUND_IMAGE:
			mnemo_draw_background_image(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_LOAD_CYR_FONTS:
			mnemo_load_cyr_fonts(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_TEXT:
			mnemo_draw_text(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_BACKGROUND_COL:
			mnemo_draw_background_col(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_GAUGE:
			mnemo_draw_gauge(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_NUMBER:
			mnemo_draw_number(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_TEXT_VAR:
			mnemo_draw_text_var(&mnemo_buf[conf_addr]);
			break;
		case MNEMO_ID_PROGR:
			mnemo_draw_progress(&mnemo_buf[conf_addr]);
			break;
	}
//	switch(elem_num) {
//		case 0:
//			if(mnemo_num==0) {
//				bt816_cmd_setbitmap(0x800000 | (8192/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
//				bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
//				bt816_cmd_dl(VERTEX2F(0, 0));
//				bt816_cmd_dl(DL_END);
//			}else if(mnemo_num==1) {
//				bt816_cmd_setbitmap(0x800000 | (69632/32), BT816_COMPRESSED_RGBA_ASTC_10x10_KHR, 800, 480);
//				bt816_cmd_dl(DL_BEGIN | BT816_BITMAPS);
//				bt816_cmd_dl(VERTEX2F(0, 0));
//				bt816_cmd_dl(DL_END);
//			}
//			break;
//		case 1:
//			mnemo_draw_lamp_image(50,50,LAMP_GRAY);
//			break;
//		case 2:
//			mnemo_draw_lamp_image(150,50,LAMP_RED);
//			break;
//		case 3:
//			mnemo_draw_lamp_image(250,50,LAMP_BLUE);
//			break;
//		case 4:
//			mnemo_draw_lamp_image(350,50,LAMP_YELLOW);
//			break;
//		case 5:
//			mnemo_draw_lamp_image(450,50,LAMP_GREEN);
//			break;
//		case 6:
//			mnemo_draw_lamp_image(550,50,LAMP_YELLOW);
//			break;
//		case 7:
//			mnemo_draw_lamp_image(650,50,LAMP_BLUE);
//			break;
//		case 8:
//			mnemo_draw_lamp_image(50,150,LAMP_RED);
//			break;
//		case 9:
//			mnemo_draw_lamp_image(150,150,LAMP_GRAY);
//			break;
//		case 10:
//			mnemo_draw_lamp_image(250,150,LAMP_RED);
//			break;
//		case 11:
//			mnemo_draw_lamp_image(350,150,LAMP_BLUE);
//			break;
//		case 12:
//			mnemo_draw_lamp_image(450,150,LAMP_YELLOW);
//			break;
//		case 13:
//			mnemo_draw_lamp_image(550,150,LAMP_GREEN);
//			break;
//		case 14:
//			mnemo_draw_lamp_image(650,150,LAMP_GRAY);
//			break;
//		case 15:
//			mnemo_draw_lamp_image(50,250,LAMP_GREEN);
//			break;
//		case 16:
//			mnemo_draw_lamp_image(150,250,LAMP_YELLOW);
//			break;
//		case 17:
//			mnemo_draw_lamp_image(250,250,LAMP_BLUE);
//			break;
//		case 18:
//			mnemo_draw_lamp_image(350,250,LAMP_RED);
//			break;
//		case 19:
//			mnemo_draw_lamp_image(450,250,LAMP_GRAY);
//			break;
//		case 20:
//			mnemo_draw_lamp_image(550,250,LAMP_RED);
//			break;
//		case 21:
//			mnemo_draw_lamp_image(650,250,LAMP_BLUE);
//			break;
//		case 22:
//			mnemo_draw_lamp_image(50,350,LAMP_YELLOW);
//			break;
//		case 23:
//			mnemo_draw_lamp_image(150,350,LAMP_GREEN);
//			break;
//		case 24:
//			mnemo_draw_lamp_image(250,350,LAMP_GRAY);
//			break;
//		case 25:
//			mnemo_draw_lamp_image(350,350,LAMP_GREEN);
//			break;
//		case 26:
//			mnemo_draw_lamp_image(450,350,LAMP_YELLOW);
//			break;
//		case 27:
//			mnemo_draw_lamp_image(550,350,LAMP_BLUE);
//			break;
//		case 28:
//			mnemo_draw_lamp_image(650,350,LAMP_RED);
//			break;
//		case 29:
//			bt816_cmd_setfont2(1,MEM_L1FONT14,32);
//			bt816_cmd_setfont2(2,MEM_L1FONT22,32);
//			bt816_cmd_setfont2(3,MEM_L1FONT30,32);
//			bt816_cmd_setfont2(4,MEM_L1FONT40,32);
//			break;
//		case 30:
//			bt816_cmd_dl(DL_COLOR_RGB | RED);
//			bt816_cmd_text(50, 100, 1, 0, "B01");
//			break;
//		case 31:
//			bt816_cmd_dl(DL_COLOR_RGB | RED);
//			bt816_cmd_text(150, 100, 2, 0, "B02");
//			break;
//		case 32:
//			bt816_cmd_dl(DL_COLOR_RGB | RED);
//			bt816_cmd_text(250, 100, 3, 0, "B03");
//			break;
//		case 33:
//			bt816_cmd_dl(DL_COLOR_RGB | RED);
//			bt816_cmd_text(350, 100, 4, 0, "B04");
//			break;
//		case 34:
//			bt816_cmd_dl(DL_COLOR_RGB | RED);
//			bt816_cmd_text(450, 100, 3, 0, "B05");
//			break;
//		case 35:
//			bt816_cmd_dl(DL_COLOR_RGB | RED);
//			bt816_cmd_text(550, 100, 2, 0, "B06");
//			break;
//		case 36:
//			bt816_cmd_dl(DL_COLOR_RGB | RED);
//			bt816_cmd_text(650, 100, 1, 0, "B07");
//			break;
//	}
}

void init_mnemo() {
	init_display_fonts();
	vTaskDelay(10);
	bt816_cmd_inflate(MEM_LAMP1, get_lamp_picture_data(LAMP_GRAY), get_lamp_picture_size(LAMP_GRAY));
	vTaskDelay(10);
	bt816_cmd_inflate(MEM_LAMP2, get_lamp_picture_data(LAMP_RED), get_lamp_picture_size(LAMP_RED));
	vTaskDelay(10);
	bt816_cmd_inflate(MEM_LAMP3, get_lamp_picture_data(LAMP_BLUE), get_lamp_picture_size(LAMP_BLUE));
	vTaskDelay(10);
	bt816_cmd_inflate(MEM_LAMP4, get_lamp_picture_data(LAMP_YELLOW), get_lamp_picture_size(LAMP_YELLOW));
	vTaskDelay(10);
	bt816_cmd_inflate(MEM_LAMP5, get_lamp_picture_data(LAMP_GREEN), get_lamp_picture_size(LAMP_GREEN));
	vTaskDelay(10);
	mnemo_cnt = 2;
}

void draw_mnemo() {


	uint16_t el_cnt = mnemo_get_elements_cnt();
	bt816_cmd_dl(CMD_DLSTART); /* start the display list */
	bt816_cmd_dl(DL_CLEAR_COLOR_RGB | BLACK);
	bt816_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	uint32_t v1 = bt816_mem_read32(REG_CMD_DL);
	uint32_t v2 = bt816_mem_read32(REG_CMDB_SPACE);

	for(uint16_t i=0;i<el_cnt;i++) {
		uint16_t offset = mnemo_buf[MNEMO_HEADER_LENGTH + i*MNEMO_ONE_ELEM_LINK_WIDTH];
		offset = offset << 8;
		offset |= mnemo_buf[MNEMO_HEADER_LENGTH + i*MNEMO_ONE_ELEM_LINK_WIDTH + 1];
		mnemo_draw_element(offset);
	}

	v1 = bt816_mem_read32(REG_CMD_DL);
	v2 = bt816_mem_read32(REG_CMDB_SPACE);

	if(plc_can_link==0) {
		uint32_t col = DL_COLOR_RGB | 0x00FFFFFF;
		bt816_cmd_dl(col);
		bt816_cmd_fgcolor(COLOR_RGB(100, 0, 0));
		bt816_cmd_button(300, 400, 200, 50, 28, 0, "CAN  ERROR");
	}

	bt816_cmd_dl(DL_DISPLAY);
	bt816_cmd_dl(CMD_SWAP);
}
