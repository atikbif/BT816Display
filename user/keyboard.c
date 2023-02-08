/*
 * keyboard.c
 *
 *  Created on: 2023 Feb 8
 *      Author: pelev
 */

#include "keyboard.h"
#include "FreeRTOS.h"
#include "task.h"
#include "at32f435_437.h"

static uint16_t key = 0;

static void keyboard_init() {
	gpio_init_type gpio_init_struct;

	/* enable the led clock */
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);

	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);

	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_13;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOC, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_1 | GPIO_PINS_3;
	gpio_init(GPIOE, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_0;
	gpio_init(GPIOF, &gpio_init_struct);

	gpio_bits_reset(GPIOC,GPIO_PINS_13);
	gpio_bits_reset(GPIOE,GPIO_PINS_1);
	gpio_bits_reset(GPIOE,GPIO_PINS_3);
	gpio_bits_reset(GPIOF,GPIO_PINS_0);

	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_7;
	gpio_init(GPIOB, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_0;
	gpio_init(GPIOE, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_15;
	gpio_init(GPIOG, &gpio_init_struct);
}

uint16_t get_pressed_keys() {
	return key;
}

void keyboard_task_function(void *pvParameters)
{
	keyboard_init();
	while(1)
	{
		uint16_t tmp = 0;

		gpio_bits_set(GPIOF,GPIO_PINS_0);
		vTaskDelay(1);
		if(gpio_input_data_bit_read(GPIOE, GPIO_PINS_0)!=RESET) tmp |= 0x0001;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_7)!=RESET) tmp |= 0x0002;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_4)!=RESET) tmp |= 0x0004;
		if(gpio_input_data_bit_read(GPIOG, GPIO_PINS_15)!=RESET) tmp |= 0x0008;
		gpio_bits_reset(GPIOF,GPIO_PINS_0);

		gpio_bits_set(GPIOC,GPIO_PINS_13);
		vTaskDelay(1);
		if(gpio_input_data_bit_read(GPIOE, GPIO_PINS_0)!=RESET) tmp |= 0x0010;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_7)!=RESET) tmp |= 0x0020;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_4)!=RESET) tmp |= 0x0040;
		if(gpio_input_data_bit_read(GPIOG, GPIO_PINS_15)!=RESET) tmp |= 0x0080;
		gpio_bits_reset(GPIOC,GPIO_PINS_13);

		gpio_bits_set(GPIOE,GPIO_PINS_3);
		vTaskDelay(1);
		if(gpio_input_data_bit_read(GPIOE, GPIO_PINS_0)!=RESET) tmp |= 0x0100;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_7)!=RESET) tmp |= 0x0200;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_4)!=RESET) tmp |= 0x0400;
		if(gpio_input_data_bit_read(GPIOG, GPIO_PINS_15)!=RESET) tmp |= 0x0800;
		gpio_bits_reset(GPIOE,GPIO_PINS_3);

		gpio_bits_set(GPIOE,GPIO_PINS_1);
		vTaskDelay(1);
		if(gpio_input_data_bit_read(GPIOE, GPIO_PINS_0)!=RESET) tmp |= 0x1000;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_7)!=RESET) tmp |= 0x2000;
		if(gpio_input_data_bit_read(GPIOB, GPIO_PINS_4)!=RESET) tmp |= 0x4000;
		if(gpio_input_data_bit_read(GPIOG, GPIO_PINS_15)!=RESET) tmp |= 0x8000;
		gpio_bits_reset(GPIOE,GPIO_PINS_1);

		key = tmp;
		vTaskDelay(16);
	}
}

