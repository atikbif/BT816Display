/*
 * appl_info_menu.h
 *
 *  Created on: 2023 May 10
 *      Author: pelev
 */

#ifndef INCLUDE_APPL_INFO_MENU_H_
#define INCLUDE_APPL_INFO_MENU_H_

#include <stdint.h>

typedef struct {
	uint16_t id;
	uint8_t name[41];
	uint8_t vers_major;
	uint8_t vers_minor;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t font_code;
	uint8_t language_code;
}appl_info_data_type;

void appl_info_menu(uint16_t key);

#endif /* INCLUDE_APPL_INFO_MENU_H_ */
