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

void lcd_task_function(void *pvParameters)
{
	bt816_init();
	//init_display2();
	//init_display_fonts();
	mnemo_read_cnt_from_config(0);
	init_mnemo();
	while(1)
	{
		//demo_display1();
		//demo_display2();
		//at32_led_toggle(LED_POW);
		//display_value(get_pressed_keys());
		//demo_display_fonts();
		draw_mnemo(0,0);
		vTaskDelay(100);
	}
}
