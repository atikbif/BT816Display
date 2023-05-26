/*
 * password_menu.h
 *
 *  Created on: 2023 May 12
 *      Author: pelev
 */

#ifndef INCLUDE_PASSWORD_MENU_H_
#define INCLUDE_PASSWORD_MENU_H_

#include <stdint.h>

enum PASSWD_CMD{CLR_ALARM_LIST=1, CONFIG_ACCESS, PASSWORD_SET1, PASSWORD_SET2};

void set_passwd_cmd(uint16_t value);
void passwd_menu(uint16_t key);
void set_passwd_value(uint8_t *ptr);

#endif /* INCLUDE_PASSWORD_MENU_H_ */
