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
#include "crc.h"

extern appl_info_data_type appl_info_data;
extern cluster_info_data_type cluster_data;
extern calc_config calc[MAX_CALC_CNT];
extern uint16_t calc_total_cnt;
extern uint8_t passwd[6];
extern ertc_time_type dev_time;

extern cluster cl;

#define MAN_VAR_CNT		15
const char* man_var_names[MAN_VAR_CNT] = {
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x31",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x32",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x33",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x34",
		"\xd0\xa2\x20\xd0\xb7\xd0\xb0\xd0\xb4\xd0\xb0\xd0\xbd\xd0\xb8\xd1\x8f\x20\x35",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x31",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x32",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x33",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x34",
		"\xd0\x9a\xd0\xbb\xd0\xb0\xd0\xbf\xd0\xb0\xd0\xbd\x20\x35",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x31",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x32",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x33",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x34",
		"\xd0\x92\xd0\xb5\xd0\xbd\xd1\x82\xd0\xb8\xd0\xbb\xd1\x8f\xd1\x82\xd0\xbe\xd1\x80\x20\x35"
};

void read_config() {
	const char appl_name[] = "test";
	appl_info_data.id = 12345;
	appl_info_data.vers_major = 1;
	appl_info_data.vers_minor = 35;
	appl_info_data.date = 15;
	appl_info_data.month = 3;
	appl_info_data.year = 23;
	memcpy(appl_info_data.name,appl_name,strlen(appl_name));

	cluster_data.used_plc[0]=1;
	cluster_data.used_plc[1]=0;
	cluster_data.used_plc[3]=0;
	cluster_data.used_plc[7]=0;
	cluster_data.plc_link[0]=0;
	cluster_data.plc_link[1]=0;
	cluster_data.plc_link[3]=0;
	cluster_data.plc_link[7]=0;
	cluster_data.plc_type[0] = PC21_1;
	//cluster_data.plc_type[1] = PC21_CD;
	//cluster_data.plc_type[3] = PC21_MC;
	//cluster_data.plc_type[7] = PC21_2T;


	ertc_calendar_get(&dev_time);

	alarm_info info;
	info.alarm_id = CHECK_EXT_FLASH_OK;
	info.time.date = dev_time.day;
	info.time.month = dev_time.month;
	info.time.year = dev_time.year;
	info.time.hour=dev_time.hour;
	info.time.min=dev_time.min;
	info.time.sec=dev_time.sec;
	add_alarm(info);

}

uint8_t get_cluster_reg_name(uint16_t num, uint8_t *buf) {
	return 0;
}

uint8_t get_cluster_bit_name(uint16_t num, uint8_t *buf) {
	return 0;
}

uint8_t get_net_reg_name(uint16_t num, uint8_t *buf) {
	return 0;
}

uint8_t get_net_bit_name(uint16_t num, uint8_t *buf) {
	return 0;
}

uint8_t get_ai_meas_unit(uint8_t dev_num, uint8_t inp_num, uint8_t *buf) {
	buf[0] = 'p';
	buf[1] = 'p';
	buf[2] = 'm';
	buf[3] = 0;

	return 3;
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

uint8_t get_inp_description(uint8_t dev_num, uint8_t inp_num, uint8_t *buf) {
	uint8_t res = 0;
	const char msg1[] = "\xd0\xa3\xd0\x94\xd0\x90\xd0\x9b\xd0\x81\xd0\x9d\xd0\x9d\xd0\xab\xd0\x99\x20\xd0\x9f\xd0\xa3\xd0\xa1\xd0\x9a";
	const char msg2[] = "\xd0\xa3\xd0\x94\xd0\x90\xd0\x9b\xd0\x81\xd0\x9d\xd0\x9d\xd0\xab\xd0\x99\x20\xd0\xa1\xd0\xa2\xd0\x9e\xd0\x9f";
	const char msg3[] = "\xd0\x9c\xd0\xb5\xd1\x81\xd1\x82\xd0\xbd\xd1\x8b\xd0\xb9\x20\xd0\xbf\xd1\x83\xd1\x81\xd0\xba";
	const char msg4[] = "\xd0\x9c\xd0\xb5\xd1\x81\xd1\x82\xd0\xbd\xd1\x8b\xd0\xb9\x20\xd1\x81\xd1\x82\xd0\xbe\xd0\xbf";
	const char msg5[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x31";
	const char msg6[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x32";
	const char msg7[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x33";
	const char msg8[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x34";
	const char msg9[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x35";
	const char msg10[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x36";
	const char msg11[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x37";
	const char msg12[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x38";
	const char msg13[] = "\xd0\x9f\xd0\xb0\xd1\x80\xd0\xb0\xd0\xbc\xd0\xb5\xd1\x82\xd1\x80\x20\x39";
	switch(inp_num) {
		case 0:
			memcpy(buf,msg1,sizeof(msg1));
			res = strlen(msg1);
			break;
		case 1:
			memcpy(buf,msg2,sizeof(msg2));
			res = strlen(msg2);
			break;
		case 2:
			memcpy(buf,msg3,sizeof(msg3));
			res = strlen(msg3);
			break;
		case 3:
			memcpy(buf,msg4,sizeof(msg4));
			res = strlen(msg4);
			break;
		case 4:
			memcpy(buf,msg5,sizeof(msg5));
			res = strlen(msg5);
			break;
		case 5:
			memcpy(buf,msg6,sizeof(msg6));
			res = strlen(msg6);
			break;
		case 6:
			memcpy(buf,msg7,sizeof(msg7));
			res = strlen(msg7);
			break;
		case 7:
			memcpy(buf,msg8,sizeof(msg8));
			res = strlen(msg8);
			break;
		case 8:
			memcpy(buf,msg9,sizeof(msg9));
			res = strlen(msg9);
			break;
		case 9:
			memcpy(buf,msg10,sizeof(msg10));
			res = strlen(msg10);
			break;
		case 10:
			memcpy(buf,msg11,sizeof(msg11));
			res = strlen(msg11);
			break;
		case 11:
			memcpy(buf,msg12,sizeof(msg12));
			res = strlen(msg12);
			break;
		case 12:
			memcpy(buf,msg13,sizeof(msg13));
			res = strlen(msg13);
			break;

	}
	return res;
}

uint8_t get_out_description(uint8_t dev_num, uint8_t out_num, uint8_t *buf) {
	uint8_t res = 0;
	const char msg1[] = "\x50\x31\x20\xd0\x9f\xd0\xa0\xd0\x9e\xd0\xa1\xd0\xab\xd0\x9f\xd0\xac";
	const char msg2[] = "\x50\x31\x20\xd0\xa1\xd0\xa5\xd0\x9e\xd0\x94\x20\xd0\x9b\xd0\x95\xd0\x9d\xd0\xa2\xd0\xab";
	const char msg3[] = "\x50\x31\x20\xd0\x9f\xd0\xa0\xd0\x9e\xd0\xa0\xd0\xab\xd0\x92\x20\xd0\x9b\xd0\x95\xd0\x9d\xd0\xa2\xd0\xab";
	switch(out_num) {
		case 0:
			memcpy(buf,msg1,sizeof(msg1));
			res = strlen(msg1);
			break;
		case 1:
			memcpy(buf,msg2,sizeof(msg2));
			res = strlen(msg2);
			break;
		case 2:
			memcpy(buf,msg3,sizeof(msg3));
			res = strlen(msg3);
			break;
	}
	return res;
}

uint16_t get_inp_cnt_with_descr(uint8_t dev_num) {
	return 13;
}

uint16_t get_out_cnt_with_descr(uint8_t dev_num) {
	return 3;
}

uint8_t get_glob_integer_name(uint16_t num, uint8_t *buf) {
	const char msg1[] = "P1 PSW TMR";
	const char msg2[] = "MULTI_MOTOR";
	const char msg3[] = "POST START";
	const char msg4[] = "RUN UP TIME";
	const char msg5[] = "CONT USED";
	const char msg6[] = "CONT IN";
	const char msg7[] = "CONT OUT";
	uint8_t res = 0;
	switch(num) {
		case 0:
			memcpy(buf,msg1,sizeof(msg1));
			res = strlen(msg1);
			break;
		case 1:
			memcpy(buf,msg2,sizeof(msg2));
			res = strlen(msg2);
			break;
		case 2:
			memcpy(buf,msg3,sizeof(msg3));
			res = strlen(msg3);
			break;
		case 3:
			memcpy(buf,msg4,sizeof(msg4));
			res = strlen(msg4);
			break;
		case 4:
			memcpy(buf,msg5,sizeof(msg5));
			res = strlen(msg5);
			break;
		case 5:
			memcpy(buf,msg6,sizeof(msg6));
			res = strlen(msg6);
			break;
		case 6:
			memcpy(buf,msg7,sizeof(msg7));
			res = strlen(msg7);
			break;
	}
	return res;
}

uint8_t get_glob_bits_name(uint16_t num, uint8_t *buf) {
	const char msg1[] = "P1 LOCKOUT";
	const char msg2[] = "P1 OPEN CIRC";
	const char msg3[] = "P1 SHORT CIRC";
	const char msg4[] = "P1 CLEAR";
	const char msg5[] = "P1 PSW FAILED";
	const char msg6[] = "P1 SCADA STOP";
	const char msg7[] = "P1 CONT FAIL";
	uint8_t res = 0;
	switch(num) {
		case 0:
			memcpy(buf,msg1,sizeof(msg1));
			res = strlen(msg1);
			break;
		case 1:
			memcpy(buf,msg2,sizeof(msg2));
			res = strlen(msg2);
			break;
		case 2:
			memcpy(buf,msg3,sizeof(msg3));
			res = strlen(msg3);
			break;
		case 3:
			memcpy(buf,msg4,sizeof(msg4));
			res = strlen(msg4);
			break;
		case 4:
			memcpy(buf,msg5,sizeof(msg5));
			res = strlen(msg5);
			break;
		case 5:
			memcpy(buf,msg6,sizeof(msg6));
			res = strlen(msg6);
			break;
		case 6:
			memcpy(buf,msg7,sizeof(msg7));
			res = strlen(msg7);
			break;
	}
	return res;
}

void read_calculation_config(const uint8_t *ptr) {
	// imitation read from flash
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

void read_password() {
	if (ertc_bpr_data_read(ERTC_DT2) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT2, 0x1234);
		ertc_bpr_data_write(ERTC_DT3, 0x0101);
		ertc_bpr_data_write(ERTC_DT4, 0x0101);
		ertc_bpr_data_write(ERTC_DT5, 0x0101);
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
	if(i<MAN_VAR_CNT) {
		res = 1;
		for(uint16_t j=0;j<40;j++) var->name[j] = 0;
		for(uint16_t j=0;j<40;j++) {
			if(j<strlen(man_var_names[i])) var->name[j] = man_var_names[i][j];
		}
		if(i<5) {
			var->link_type = VAR_LINK_CL_REG;
			var->link_index = i+10;
			var->min = 0;
			var->max = 15000;
		}else if(i<10) {
			var->link_type = VAR_LINK_CL_BIT;
			var->link_index = i-5;
			var->min = 0;
			var->max = 1;
		}else {
			var->link_type = VAR_LINK_NET_BIT;
			var->link_index = i-10;
			var->min = 0;
			var->max = 1;
		}
		var->var_cnt = MAN_VAR_CNT;
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
		if(length>4096) length = 4096;
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
