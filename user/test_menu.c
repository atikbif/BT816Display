/*
 * test_appl_info_menu.c
 *
 *  Created on: 2023 May 10
 *      Author: pelev
 */

#include "test_menu.h"
#include "appl_info_menu.h"
#include "cluster_state_menu.h"
#include "alarm_list_menu.h"
#include "password_menu.h"
#include "clear_alarms_menu.h"
#include "diagnostic_menu.h"
#include "pc21_state_menu.h"
#include "cross_reference_menu.h"
#include "global_integers_menu.h"
#include "menu_list.h"
#include <string.h>

extern menu_list_t current_menu;
extern appl_info_data_type appl_info_data;
extern cluster_info_data_type cluster_data;

void test_appl_info_menu() {
	const char appl_name[] = "test";
	appl_info_data.id = 12345;
	appl_info_data.vers_major = 1;
	appl_info_data.vers_minor = 35;
	appl_info_data.date = 15;
	appl_info_data.month = 3;
	appl_info_data.year = 23;
	memcpy(appl_info_data.name,appl_name,strlen(appl_name));
	current_menu = MENU_APPL_INFO;
}

void test_cluster_info_menu() {
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
	current_menu = MENU_CLUST_INFO;
}

void test_alarm_list_menu() {
	for(uint8_t i=0;i<70;i++) {
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
	current_menu = MENU_ALARM_LIST;
}

void test_passwd_menu() {
	set_passwd_cmd(CLR_ALARM_LIST);
	current_menu = MENU_PASSWD;
}

void test_clear_alarms_menu() {
	current_menu = MENU_CLR_ALARMS;
}

void test_diagnostic_menu() {
	current_menu = MENU_DIAGNOSTIC;
}

void test_pc21_state_menu() {
	current_menu = MENU_PC21_STATE;
}

void test_cross_ref_menu() {
	current_menu = MENU_CROSS_REF;
}

void test_glob_integers_menu() {
	current_menu = MENU_GLOB_INT;
}
