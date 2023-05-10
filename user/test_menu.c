/*
 * test_appl_info_menu.c
 *
 *  Created on: 2023 May 10
 *      Author: pelev
 */

#include "test_menu.h"
#include "appl_info_menu.h"
#include "menu_list.h"
#include <string.h>

extern menu_list_t current_menu;
extern appl_info_data_type appl_info_data;

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
