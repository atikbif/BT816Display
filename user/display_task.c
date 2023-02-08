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

void lcd_task_function(void *pvParameters)
{
	vTaskDelay(200);
	bt816_init();
	init_display2();
	while(1)
	{
		demo_display2();
		//at32_led_toggle(LED_POW);
		vTaskDelay(100);
	}
}
