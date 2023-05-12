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

void lcd_task_function(void *pvParameters)
{
	uint16_t prev_key = 0;
	uint16_t key_cmd = 0;
	uint16_t tmr = 0;
	uint8_t mnemo_num = 0;
	bt816_init();
	read_config();
	init_display_fonts();
	//init_mnemo();
	//read_mnemo_data(mnemo_num);
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
