/*
 * backlight.c
 *
 *  Created on: 2022 Sep 14
 *      Author: Roman
 */

#include "backlight.h"
#include "at32f435_437_board.h"

void lcd_power_enable(uint8_t value) {
	if(value) gpio_bits_write(GPIOA, GPIO_PINS_11, TRUE);
	else gpio_bits_write(GPIOA, GPIO_PINS_11, FALSE);
}

void lcd_backlight_enable(uint8_t value) {
	if(value) gpio_bits_write(GPIOE, GPIO_PINS_14, TRUE);
	else gpio_bits_write(GPIOE, GPIO_PINS_14, FALSE);
}

void init_backlight() {
	gpio_init_type gpio_init_struct;

	/* enable the pin clock */
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);

	/* lcd power enable */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_11;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_init_struct);
	lcd_power_enable(1);

	/* backlight enable */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_14;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOE, &gpio_init_struct);
	lcd_backlight_enable(1);

	/* dim enable */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_15;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOE, &gpio_init_struct);
	gpio_bits_write(GPIOE, GPIO_PINS_15, TRUE);

	/* load 1 */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_14;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOB, &gpio_init_struct);
	gpio_bits_write(GPIOB, GPIO_PINS_14, FALSE);

	/* load 2 */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_15;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOB, &gpio_init_struct);
	gpio_bits_write(GPIOB, GPIO_PINS_15, FALSE);

	/* load 3 */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_8;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_bits_write(GPIOD, GPIO_PINS_8, FALSE);

	/* load 4 */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_9;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_bits_write(GPIOD, GPIO_PINS_9, FALSE);

	/* load 5 */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_10;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_bits_write(GPIOD, GPIO_PINS_10, FALSE);

	/* load 6 */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_11;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_bits_write(GPIOD, GPIO_PINS_11, FALSE);

	set_load(1,1);
	set_load(2,1);
	set_load(3,1);
	set_load(4,1);
	set_load(5,1);
	set_load(6,1);
}

void set_load(uint8_t num, uint8_t value) {
	switch(num) {
		case 1:
			if(value) gpio_bits_write(GPIOB, GPIO_PINS_14, TRUE);
			else gpio_bits_write(GPIOB, GPIO_PINS_14, FALSE);
			break;
		case 2:
			if(value) gpio_bits_write(GPIOB, GPIO_PINS_15, TRUE);
			else gpio_bits_write(GPIOB, GPIO_PINS_15, FALSE);
			break;
		case 3:
			if(value) gpio_bits_write(GPIOD, GPIO_PINS_8, TRUE);
			else gpio_bits_write(GPIOD, GPIO_PINS_8, FALSE);
			break;
		case 4:
			if(value) gpio_bits_write(GPIOD, GPIO_PINS_9, TRUE);
			else gpio_bits_write(GPIOD, GPIO_PINS_9, FALSE);
			break;
		case 5:
			if(value) gpio_bits_write(GPIOD, GPIO_PINS_10, TRUE);
			else gpio_bits_write(GPIOD, GPIO_PINS_10, FALSE);
			break;
		case 6:
			if(value) gpio_bits_write(GPIOD, GPIO_PINS_11, TRUE);
			else gpio_bits_write(GPIOD, GPIO_PINS_11, FALSE);
			break;
	}
}
