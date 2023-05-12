/*
 * alarm_list_menu.h
 *
 *  Created on: 2023 May 11
 *      Author: pelev
 */

#ifndef INCLUDE_ALARM_LIST_MENU_H_
#define INCLUDE_ALARM_LIST_MENU_H_

#include <stdint.h>
#include "time.h"

enum {CHECK_EXT_FLASH_OK=1, CHECK_EXT_FLASH_ERR};

#define ALARM_LIST_SIZE		128

typedef struct {
	uint16_t alarm_id;
	time_info time;
	uint32_t additional_info_addr;
}alarm_info;

void add_alarm(alarm_info info);
void alarm_info_menu(uint16_t key);
void clear_alarm_list();

#endif /* INCLUDE_ALARM_LIST_MENU_H_ */
