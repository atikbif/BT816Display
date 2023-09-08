/*
 * config.c
 *
 *  Created on: 2023 May 12
 *      Author: pelev
 */

#include "config.h"
#include <stdint.h>
#include "appl_info_menu.h"
#include "cluster_state_menu.h"
#include "alarm_list_menu.h"
#include "password_menu.h"
#include "plc_data.h"
#include <string.h>
#include "at32f435_437_board.h"
#include "var_link.h"
#include "cluster_state.h"
#include "message_scaner.h"
#include "message_archive.h"
#include "crc.h"
#include "bt816_cmd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cur_time.h"

extern appl_info_data_type appl_info_data;
extern cluster_info_data_type cluster_data;
extern calc_config calc[MAX_CALC_CNT];
extern struct bit_message_conf msg_conf[MAX_MSG_CNT];
extern uint16_t calc_total_cnt;
extern uint8_t passwd[6];
extern ertc_time_type dev_time;

uint32_t net_reg_names_addr = 0;
uint16_t net_reg_names_cnt = 0;
uint32_t net_bit_names_addr = 0;
uint16_t net_bit_names_cnt = 0;
uint32_t clust_reg_names_addr = 0;
uint16_t clust_reg_names_cnt = 0;
uint32_t clust_bit_names_addr = 0;
uint16_t clust_bit_names_cnt = 0;
uint32_t edit_var_addr = 0;
uint16_t edit_var_cnt = 0;
uint32_t message_var_addr = 0;
uint32_t message_var_cnt = 0;
uint32_t conf_ai_addr = 0;
uint16_t conf_ai_cnt = 0;
uint32_t inp_descr_addr = 0;
uint16_t inp_descr_cnt = 0;
uint32_t out_descr_addr = 0;
uint16_t out_descr_cnt = 0;

uint8_t conf_buf[4096];

uint8_t check_config_result = 0;

extern cluster cl;

extern uint8_t lcd_can_addr;

void read_config() {

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	check_config_result = 0;
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(0,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 0)) {
			check_config_result = 1;
		}
	}
	if(check_config_result) {
		appl_info_data.id = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		appl_info_data.vers_major = conf_buf[8];
		appl_info_data.vers_minor = conf_buf[9];
		appl_info_data.date = conf_buf[10];
		appl_info_data.month = conf_buf[11];
		appl_info_data.year = conf_buf[12];
		for(int i=0;i<41;i++) appl_info_data.name[40] = 0;
		for(int i=0;i<40;i++) appl_info_data.name[i] = conf_buf[18+i];
	}else {
		const char appl_name[] = "bad config";
		appl_info_data.id = 12345;
		appl_info_data.vers_major = 1;
		appl_info_data.vers_minor = 35;
		appl_info_data.date = 15;
		appl_info_data.month = 3;
		appl_info_data.year = 23;
		memcpy(appl_info_data.name,appl_name,strlen(appl_name));
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(10,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 10)) {
			lcd_can_addr = conf_buf[6];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(5,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 5)) {
			net_reg_names_addr = 4096 + addr + 64;
			net_reg_names_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(6,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 6)) {
			clust_reg_names_addr = 4096 + addr + 64;
			clust_reg_names_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(7,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 7)) {
			net_bit_names_addr = 4096 + addr + 64;
			net_bit_names_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(8,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 8)) {
			clust_bit_names_addr = 4096 + addr + 64;
			clust_bit_names_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(15,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 15)) {
			edit_var_addr = 4096 + addr + 64;
			edit_var_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(16,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 16)) {
			message_var_addr = 4096 + addr + 64;
			message_var_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(2,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 2)) {
			conf_ai_addr = 4096 + addr + 64;
			conf_ai_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(11,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 11)) {
			inp_descr_addr = 4096 + addr + 64;
			inp_descr_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(12,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 12)) {
			out_descr_addr = 4096 + addr + 64;
			out_descr_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
		}
	}

	cluster_data.used_plc[0]=1;
	cluster_data.used_plc[1]=0;
	cluster_data.used_plc[3]=0;
	cluster_data.used_plc[7]=0;
	cluster_data.plc_link[0]=0;
	cluster_data.plc_link[1]=0;
	cluster_data.plc_link[3]=0;
	cluster_data.plc_link[7]=0;
	cluster_data.plc_type[0] = PC21_1;

	ertc_calendar_get(&dev_time);


	struct message_record rec;
	rec.message_type = 0;
	rec.message_id = MSG_ARCH_CHECK_FLASH;
	rec.time = time_to_uint32();
	uint8_t rec_body[1];
	rec_body[0] = 1; //test ok
	rec.length = 1;
	rec.ptr = rec_body;
	add_record_to_archive(&rec);

}

uint8_t get_cluster_reg_name(uint16_t num, uint8_t *buf, uint16_t max_length) {
	uint8_t res = 0;
	if(clust_reg_names_addr && (num < clust_reg_names_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, clust_reg_names_addr+128ul*num, 128);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i+64); // 64 - user name offset
		}
		if(user_name[0]==0) {
			for(uint16_t i=0;i<40;i++) {
				user_name[i] = bt816_mem_read8(i); // 0 - sys name offset
			}
		}
		uint16_t length = strlen(user_name);
		if(length>=max_length) {
			length = max_length-1;
			user_name[max_length-1]=0;
		}
		memcpy(buf,user_name,length+1);
		res = length;
	}
	return res;
}

uint8_t get_cluster_bit_name(uint16_t num, uint8_t *buf, uint16_t max_length) {
	uint8_t res = 0;
	if(clust_bit_names_addr && (num < clust_bit_names_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, clust_bit_names_addr+128ul*num, 128);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i+64); // 64 - user name offset
		}
		if(user_name[0]==0) {
			for(uint16_t i=0;i<40;i++) {
				user_name[i] = bt816_mem_read8(i); // 0 - sys name offset
			}
		}
		uint16_t length = strlen(user_name);
		if(length>=max_length) {
			length = max_length-1;
			user_name[max_length-1]=0;
		}
		memcpy(buf,user_name,length+1);
		res = length;
	}
	return res;
}

uint8_t get_net_reg_name(uint16_t num, uint8_t *buf, uint16_t max_length) {
	uint8_t res = 0;
	if(net_reg_names_addr && (num < net_reg_names_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, net_reg_names_addr+128ul*num, 128);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i+64); // 64 - user name offset
		}
		if(user_name[0]==0) {
			for(uint16_t i=0;i<40;i++) {
				user_name[i] = bt816_mem_read8(i); // 0 - sys name offset
			}
		}
		uint16_t length = strlen(user_name);
		if(length>=max_length) {
			length = max_length-1;
			user_name[max_length-1]=0;
		}
		memcpy(buf,user_name,length+1);
		res = length;
	}
	return res;
}

uint8_t get_net_bit_name(uint16_t num, uint8_t *buf, uint16_t max_length) {
	uint8_t res = 0;
	if(net_bit_names_addr && (num < net_bit_names_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, net_bit_names_addr+128ul*num, 128);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i+64); // 64 - user name offset
		}
		if(user_name[0]==0) {
			for(uint16_t i=0;i<40;i++) {
				user_name[i] = bt816_mem_read8(i); // 0 - sys name offset
			}
		}
		uint16_t length = strlen(user_name);
		if(length>=max_length) {
			length = max_length-1;
			user_name[max_length-1]=0;
		}
		memcpy(buf,user_name,length+1);
		res = length;
	}
	return res;
}

uint8_t get_ai_meas_unit(uint8_t dev_num, uint8_t inp_num, uint8_t *buf) {
	uint8_t res = 0;

	if(conf_ai_addr && (inp_num < conf_ai_cnt)) {
		uint8_t user_name[21];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, conf_ai_addr+192ul*inp_num, 192);
		vTaskDelay(1);
		for(uint16_t i=0;i<20;i++) {
			user_name[i] = bt816_mem_read8(i+128); // 128 - measure unit name offset
		}
		memcpy(buf,user_name,sizeof(user_name));
		res = strlen(user_name);
	}
	return res;
}

uint8_t get_inp_name(uint8_t dev_num, uint8_t inp_num, uint8_t *buf) {
	buf[0] = 'I';
	buf[1] = 'N';
	buf[2] = 'P';
	buf[3] = (inp_num/10)+'0';
	buf[4] = (inp_num%10) + '0';
	buf[5] = 0;
	return 5;
}

uint8_t get_inp_description(uint8_t dev_num, uint8_t inp_num, uint8_t *name_buf, uint8_t *descr_buf) {
	uint8_t res = 0;

	if(inp_descr_addr && (inp_num < inp_descr_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, inp_descr_addr+64ul*inp_num, 64);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i);
		}
		memcpy(name_buf,user_name,sizeof(user_name));
		res = strlen(user_name);

		uint8_t inp_type = bt816_mem_read8(40);
		if(inp_type==0) { // DI
			descr_buf[0] = 0x30 + dev_num;
			descr_buf[1] = 'D';
			descr_buf[2] = (inp_num+1)/10 + '0';
			descr_buf[3] = (inp_num+1)%10 + '0';
			descr_buf[4] = 0;
		}else { // AI
			descr_buf[0] = 0x30 + dev_num;
			descr_buf[1] = 'A';
			descr_buf[2] = (inp_num+1)/10 + '0';
			descr_buf[3] = (inp_num+1)%10 + '0';
			descr_buf[4] = 0;
		}
	}
	return res;
}

uint8_t get_out_description(uint8_t dev_num, uint8_t out_num, uint8_t *buf) {
	uint8_t res = 0;

	if(out_descr_addr && (out_num < out_descr_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, out_descr_addr+64ul*out_num, 64);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i);
		}
		memcpy(buf,user_name,sizeof(user_name));
		res = strlen(user_name);
	}
	return res;
}

uint16_t get_inp_cnt_with_descr(uint8_t dev_num) {
	uint16_t res = 0;
	uint8_t user_name[41];
	if(inp_descr_addr) {
		for(int i=0;i<inp_descr_cnt;i++) {
			bt816_cmd_flashread(0, inp_descr_addr+64ul*i, 64);
			vTaskDelay(1);
			for(uint16_t i=0;i<40;i++) {
				user_name[i] = bt816_mem_read8(i);
			}
			if(user_name[0]!=0) res++;
		}
	}
	return res;
}

uint16_t get_out_cnt_with_descr(uint8_t dev_num) {
	uint16_t res = 0;
	uint8_t user_name[41];
	if(out_descr_addr) {
		for(int i=0;i<out_descr_cnt;i++) {
			bt816_cmd_flashread(0, out_descr_addr+64ul*i, 64);
			vTaskDelay(1);
			for(uint16_t i=0;i<40;i++) {
				user_name[i] = bt816_mem_read8(i);
			}
			if(user_name[0]!=0) res++;
		}
	}
	return res;
}

uint8_t get_glob_integer_name(uint16_t num, uint8_t *buf) {
	uint8_t res = 0;

	if(net_reg_names_addr && (num < net_reg_names_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, net_reg_names_addr+64+128ul*num, 64);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i);
		}
		memcpy(buf,user_name,sizeof(user_name));
		res = strlen(user_name);
	}
	return res;
}

uint8_t get_glob_bits_name(uint16_t num, uint8_t *buf) {
	uint8_t res = 0;
	if(net_bit_names_addr && (num < net_bit_names_cnt)) {
		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, net_bit_names_addr+64+128ul*num, 64);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i);
		}
		memcpy(buf,user_name,sizeof(user_name));
		res = strlen(user_name);
	}
	return res;
}

void read_calculation_config(const uint8_t *ptr) {
	bt816_cmd_flashread(0, 4096, 4096);
	vTaskDelay(1);
	for(uint16_t i=0;i<4096;i++) {
		conf_buf[i] = bt816_mem_read8(i);
	}
	if(check_config_header(conf_buf)) {
		uint32_t addr = get_config_offset_by_id(9,conf_buf);
		bt816_cmd_flashread(0, 4096 + addr, 4096);
		vTaskDelay(1);
		for(uint16_t i=0;i<4096;i++) {
			conf_buf[i] = bt816_mem_read8(i);
		}
		if(check_item_config(conf_buf, 9)) {
			calc_total_cnt = ((uint16_t)conf_buf[6]<<8) | conf_buf[7];
			if(calc_total_cnt>=PC21_INP_CNT) calc_total_cnt = PC21_INP_CNT;
			for (uint16_t i=0;i<calc_total_cnt;i++) {
				calc[i].index = ((uint16_t)conf_buf[8 + i*12]<<8) | conf_buf[9 + i*12];
				calc[i].link = conf_buf[10 + i*12];
				if(calc[i].link>=CALC_LAST_LINK) calc[i].link = LINK_RAW;
				calc[i].prec = conf_buf[11 + i*12];
				if(calc[i].prec > PR3) calc[i].prec = PR3;

				union {
					float a;
					uint8_t bytes[4];
				} float_converter;

				float_converter.bytes[0] = conf_buf[12 + i*12];
				float_converter.bytes[1] = conf_buf[13 + i*12];
				float_converter.bytes[2] = conf_buf[14 + i*12];
				float_converter.bytes[3] = conf_buf[15 + i*12];
				calc[i].k = float_converter.a;

				float_converter.bytes[0] = conf_buf[16 + i*12];
				float_converter.bytes[1] = conf_buf[17 + i*12];
				float_converter.bytes[2] = conf_buf[18 + i*12];
				float_converter.bytes[3] = conf_buf[19 + i*12];
				calc[i].b = float_converter.a;

			}
		}else {
			calc_total_cnt = 14;
			for(uint16_t i=0;i<calc_total_cnt;i++) {
				calc[i].k = 1.0;
				calc[i].b = 0;
				calc[i].link = LINK_RAW;
				calc[i].index = i;
				calc[i].result = 0;
				calc[i].prec = PR1;
			}
		}
	}

}

void read_message_conf() {
	for(uint16_t i=0;i<MAX_MSG_CNT;i++) {
		msg_conf[i].used = 0;
		msg_conf[i].prev_value = 0;
		msg_conf[i].value = 0;

		if(message_var_addr && (i < message_var_cnt)) {
			bt816_cmd_flashread(0, message_var_addr+64ul*i, 64);
			vTaskDelay(5);
			msg_conf[i].var_type = bt816_mem_read8(40);
			msg_conf[i].var_index = bt816_mem_read8(41);
			msg_conf[i].var_index = msg_conf[i].var_index << 8;
			msg_conf[i].var_index |= bt816_mem_read8(42);
			msg_conf[i].message_type = bt816_mem_read8(43);
			msg_conf[i].used = 1;
			vTaskDelay(1);
		}
	}
}

uint8_t get_message_name(uint16_t num, uint8_t *buf) {
	uint8_t res  = 0;
	if(message_var_addr && num<message_var_cnt) {

		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, message_var_addr+64ul*num, 64);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i);
		}
		memcpy(buf,user_name,sizeof(user_name));
		res = strlen(user_name);
	}
	return res;
}

void read_ip_addr(uint8_t *ptr) {
	if (ertc_bpr_data_read(ERTC_DT17) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT17, 0x1234);
		ertc_bpr_data_write(ERTC_DT18, 0xC0A80102);
		ertc_bpr_data_write(ERTC_DT19, 0xFFFFFF00);
		ertc_bpr_data_write(ERTC_DT20, 0xC0A80101);
	}
	uint32_t val  = ertc_bpr_data_read(ERTC_DT18);
	ptr[0] = (val>>24)&0xFF;
	ptr[1] = (val>>16)&0xFF;
	ptr[2] = (val>>8)&0xFF;
	ptr[3] = (val>>0)&0xFF;
}

void read_ip_mask(uint8_t *ptr) {
	if (ertc_bpr_data_read(ERTC_DT17) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT17, 0x1234);
		ertc_bpr_data_write(ERTC_DT18, 0xC0A80102);
		ertc_bpr_data_write(ERTC_DT19, 0xFFFFFF00);
		ertc_bpr_data_write(ERTC_DT20, 0xC0A80101);
	}
	uint32_t val  = ertc_bpr_data_read(ERTC_DT19);
	ptr[0] = (val>>24)&0xFF;
	ptr[1] = (val>>16)&0xFF;
	ptr[2] = (val>>8)&0xFF;
	ptr[3] = (val>>0)&0xFF;
}

void read_ip_gate(uint8_t *ptr) {
	if (ertc_bpr_data_read(ERTC_DT17) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT17, 0x1234);
		ertc_bpr_data_write(ERTC_DT18, 0xC0A80102);
		ertc_bpr_data_write(ERTC_DT19, 0xFFFFFF00);
		ertc_bpr_data_write(ERTC_DT20, 0xC0A80101);
	}
	uint32_t val  = ertc_bpr_data_read(ERTC_DT20);
	ptr[0] = (val>>24)&0xFF;
	ptr[1] = (val>>16)&0xFF;
	ptr[2] = (val>>8)&0xFF;
	ptr[3] = (val>>0)&0xFF;
}

void set_ip_addr(uint8_t *ptr) {
	if (ertc_bpr_data_read(ERTC_DT17) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT17, 0x1234);
		ertc_bpr_data_write(ERTC_DT18, 0xC0A80102);
		ertc_bpr_data_write(ERTC_DT19, 0xFFFFFF00);
		ertc_bpr_data_write(ERTC_DT20, 0xC0A80101);
	}
	uint32_t val  = ptr[0];
	val = val << 8;
	val |= ptr[1];
	val = val << 8;
	val |= ptr[2];
	val = val << 8;
	val |= ptr[3];
	ertc_bpr_data_write(ERTC_DT18, val);
}

void set_ip_mask(uint8_t *ptr) {
	if (ertc_bpr_data_read(ERTC_DT17) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT17, 0x1234);
		ertc_bpr_data_write(ERTC_DT18, 0xC0A80102);
		ertc_bpr_data_write(ERTC_DT19, 0xFFFFFF00);
		ertc_bpr_data_write(ERTC_DT20, 0xC0A80101);
	}
	uint32_t val  = ptr[0];
	val = val << 8;
	val |= ptr[1];
	val = val << 8;
	val |= ptr[2];
	val = val << 8;
	val |= ptr[3];
	ertc_bpr_data_write(ERTC_DT19, val);
}

void set_ip_gate(uint8_t *ptr) {
	if (ertc_bpr_data_read(ERTC_DT17) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT17, 0x1234);
		ertc_bpr_data_write(ERTC_DT18, 0xC0A80102);
		ertc_bpr_data_write(ERTC_DT19, 0xFFFFFF00);
		ertc_bpr_data_write(ERTC_DT20, 0xC0A80101);
	}
	uint32_t val  = ptr[0];
	val = val << 8;
	val |= ptr[1];
	val = val << 8;
	val |= ptr[2];
	val = val << 8;
	val |= ptr[3];
	ertc_bpr_data_write(ERTC_DT20, val);
}

void read_password() {
	if (ertc_bpr_data_read(ERTC_DT2) != 0x1235)
	{
		ertc_bpr_data_write(ERTC_DT2, 0x1235);
		ertc_bpr_data_write(ERTC_DT3, 0x0102);
		ertc_bpr_data_write(ERTC_DT4, 0x0304);
		ertc_bpr_data_write(ERTC_DT5, 0x0506);
	}
	uint16_t val  = ertc_bpr_data_read(ERTC_DT3);
	passwd[0] = val >> 8;
	passwd[1] = val & 0xFF;
	val  = ertc_bpr_data_read(ERTC_DT4);
	passwd[2] = val >> 8;
	passwd[3] = val & 0xFF;
	val  = ertc_bpr_data_read(ERTC_DT5);
	passwd[4] = val >> 8;
	passwd[5] = val & 0xFF;
}

uint8_t get_manage_var(uint16_t i, manage_var *var) {
	uint8_t res = 0;
	if(edit_var_addr && i<edit_var_cnt) {

		uint8_t user_name[41];
		for(uint16_t i=0;i<sizeof(user_name);i++) user_name[i]=0;
		bt816_cmd_flashread(0, edit_var_addr+64ul*i, 64);
		vTaskDelay(1);
		for(uint16_t i=0;i<40;i++) {
			user_name[i] = bt816_mem_read8(i);
		}
		memcpy(var->name,user_name,sizeof(user_name));
		uint8_t link_type = bt816_mem_read8(40);

		switch(link_type) {
			case 0:
				var->link_type = VAR_LINK_CL_BIT;
				break;
			case 1:
				var->link_type = VAR_LINK_CL_REG;
				break;
			case 2:
				var->link_type = VAR_LINK_NET_BIT;
				break;
			case 3:
				var->link_type = VAR_LINK_NET_REG;
				break;
		}

		var->link_index = bt816_mem_read8(41);
		var->link_index = var->link_index << 8;
		var->link_index |= bt816_mem_read8(42);

		var->min = bt816_mem_read8(43);
		var->min = var->min << 8;
		var->min |= bt816_mem_read8(44);

		var->max = bt816_mem_read8(45);
		var->max = var->max << 8;
		var->max |= bt816_mem_read8(46);

		var->var_cnt = edit_var_cnt;
		var->value = 0;
		switch(var->link_type) {
			case VAR_LINK_CL_REG:
				if(var->link_index<CLUSTER_REGS_CNT) var->value = cl.cluster_regs[var->link_index];
				break;
			case VAR_LINK_NET_REG:
				if(var->link_index<NET_REGS_CNT) var->value = cl.net_regs[var->link_index];
				break;
			case VAR_LINK_CL_BIT:
				if(var->link_index<CLUST_BIT_CNT) var->value = cl.cluster_bits[var->link_index];
				break;
			case VAR_LINK_NET_BIT:
				if(var->link_index<NET_BITS_CNT) var->value = cl.net_bits[var->link_index];
				break;
		}

		res = 1;
	}
	return res;
}

uint8_t check_config_header(uint8_t *ptr) {
	if((ptr[0]==0x0D)&&(ptr[1]==0x0A)&&(ptr[2]==0x0A)&&(ptr[3]==0x0D)) {
		uint16_t length = (uint16_t)ptr[4]<<8;
		length |= ptr[5];
		if(length>4096) length = 4096;
		if(length<2) length = 2;
		length-=2;
		ptr[4] = 0;
		ptr[5] = 0;
		uint16_t crc = GetCRC16(ptr, length);
		if((ptr[length] == (crc&0xFF))&&(ptr[length+1] == (crc>>8))) {
			return 1;
		}
	}
	return 0;
}

uint32_t get_config_param_by_id(uint16_t id,uint8_t *header) {
	const uint16_t offset = 6;
	const uint16_t max_length = 4096;
	const uint16_t step = 10;
	uint32_t res = 0;
	uint16_t pos = offset;
	while(pos<max_length) {
		uint16_t cur_id = ((uint16_t)header[pos]<<8)|header[pos+1];
		if(cur_id==id) {
			res = header[pos+2];
			res = res << 8;
			res |= header[pos+3];
			res = res << 8;
			res |= header[pos+4];
			res = res << 8;
			res |= header[pos+5];
			break;
		}
		pos+=step;
	}
	return res;
}

uint32_t get_config_offset_by_id(uint16_t id, uint8_t *header) {
	const uint16_t offset = 6;
	const uint16_t max_length = 4096;
	const uint16_t step = 10;
	uint32_t res = 0;
	uint16_t pos = offset;
	while(pos<max_length) {
		uint16_t cur_id = ((uint16_t)header[pos]<<8)|header[pos+1];
		if(cur_id==id) {
			res = header[pos+6];
			res = res << 8;
			res |= header[pos+7];
			res = res << 8;
			res |= header[pos+8];
			res = res << 8;
			res |= header[pos+9];
			break;
		}
		pos+=step;
	}
	return res;
}

uint8_t check_item_config(uint8_t *ptr, uint16_t id) {
	uint8_t res = 0;
	uint16_t cur_id = ((uint16_t)ptr[0]<<8) | ptr[1];
	if(cur_id == id) {
		uint16_t length = ((uint16_t)ptr[2]<<8) | ptr[3];
		if(length>4096) return 1; // no crc check for big buffers
		if(length<2) length = 2;
		length-=2;
		ptr[2] = 0;
		ptr[3] = 0;
		uint16_t crc = GetCRC16(ptr, length);
		if((ptr[length] == (crc&0xFF))&&(ptr[length+1] == (crc>>8))) {
			return 1;
		}
	}
	return res;
}

uint16_t get_item_conf_version(uint8_t *ptr) {
	return ((uint16_t)ptr[4]<<8) | ptr[5];
}
