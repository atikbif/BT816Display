/*
 * display_task.c
 *
 *  Created on: 2023 Feb 8
 *      Author: pelev
 */

#include "display_task.h"
#include "demo_display.h"
#include "FreeRTOS.h"
#include "task.h"
#include "backlight.h"
#include "bt816_cmd.h"
#include "keyboard.h"
#include "mnemo.h"
#include "menu.h"
#include "config.h"

#include "plc_data.h"
#include "can.h"
#include "cluster_state.h"
#include "cur_time.h"
#include "trend_data.h"
#include "cluster_state_menu.h"
#include "bt816_spi.h"

extern cluster cl;

void lcd_task_function(void *pvParameters)
{
	uint16_t prev_key = 0;
	uint16_t key_cmd = 0;
	uint16_t tmr = 0;
	uint8_t mnemo_num = 0;

	init_backlight();
	vTaskDelay(500);
	bt816_spi_init();
	init_plc_data();

	can1_init();
	init_cluster(&cl);
	init_cur_time();
	init_trends();

	uint8_t try = 0;
	while(try<5) {
		if(bt816_init()) break;
		vTaskDelay(100);
		try++;
	}
	init_display_fonts();
	init_mnemo();
	read_config();
	read_calculation_config(0);
	read_password();
	read_mnemo_data(mnemo_num);
	init_menu();
	while(1)
	{
		//draw_mnemo();

		uint16_t cur_key = get_pressed_keys();
		if(cur_key && (cur_key!=prev_key)) {
			key_cmd = cur_key;
		}else key_cmd = 0;
		prev_key = cur_key;

		display_menu(key_cmd);
		//demo_display_fonts();
		vTaskDelay(100);
	}
}
