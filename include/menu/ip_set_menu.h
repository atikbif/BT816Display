/*
 * ip_set_menu.h
 *
 *  Created on: 2023 Jul 20
 *      Author: pelev
 */

#ifndef INCLUDE_MENU_IP_SET_MENU_H_
#define INCLUDE_MENU_IP_SET_MENU_H_

enum {IP_TYPE_ADDR, IP_TYPE_MASK, IP_TYPE_GATE};

#include <stdint.h>

void set_ip_type(uint8_t value);
void ip_set_menu(uint16_t key);

#endif /* INCLUDE_MENU_IP_SET_MENU_H_ */
