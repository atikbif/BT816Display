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
#include <string.h>

extern appl_info_data_type appl_info_data;
extern cluster_info_data_type cluster_data;

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
	cluster_data.used_plc[1]=1;
	cluster_data.used_plc[3]=1;
	cluster_data.used_plc[7]=1;
	cluster_data.plc_link[0]=1;
	cluster_data.plc_link[1]=1;
	cluster_data.plc_link[3]=1;
	cluster_data.plc_link[7]=0;
	cluster_data.plc_type[0] = PC21_1;
	cluster_data.plc_type[1] = PC21_CD;
	cluster_data.plc_type[3] = PC21_MC;
	cluster_data.plc_type[7] = PC21_2T;

	for(uint8_t i=0;i<3;i++) {
		alarm_info info;
		info.alarm_id = CHECK_EXT_FLASH_OK;
		info.time.date = 1;
		info.time.month = 2;
		info.time.year = 23;
		info.time.hour=14;
		info.time.min=27+i/60;
		info.time.sec=i%60;
		add_alarm(info);
	}

	uint8_t passwd[6]={1,1,1,1,1,1};
	set_passwd_value(passwd);
}
