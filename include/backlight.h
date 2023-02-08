/*
 * backlight.h
 *
 *  Created on: 2022 Sep 14
 *      Author: Roman
 */

#ifndef INCLUDE_BACKLIGHT_H_
#define INCLUDE_BACKLIGHT_H_

#include <stdio.h>

void init_backlight();
void lcd_power_enable(uint8_t value);
void set_load(uint8_t num, uint8_t value);

#endif /* INCLUDE_BACKLIGHT_H_ */
