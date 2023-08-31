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
#include "message_archive.h"
#include "message_scaner.h"
#include "test_menu.h"

extern cluster cl;
extern uint8_t plc_can_link;

volatile uint8_t err_str[128];

uint16_t stop_display = 0;

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
	init_trends();

	uint8_t try = 0;
	while(try<5) {
		if(bt816_init()) break;
		vTaskDelay(100);
		try++;
	}
	init_archive();
	init_display_fonts();
	init_mnemo();
	read_config();
	read_calculation_config(0);
	read_password();
	read_mnemo_data(mnemo_num);
	read_message_conf();
	init_menu();

	//test_alarm_list_menu();
	if(plc_can_link==0) {
		struct message_record rec;
		rec.message_type = 0;
		rec.message_id = MSG_ARCH_CHECK_CAN;
		rec.time = time_to_uint32();
		uint8_t rec_body[1];
		rec_body[0] = 0; //can error
		rec.length = 1;
		rec.ptr = rec_body;
		add_record_to_archive(&rec);
	}

	while(1)
	{
		//draw_mnemo();


		scan_messages();

		uint32_t space = bt816_mem_read32(REG_CMDB_SPACE);
		if((space & 0x3) != 0) {


			for(uint8_t i=0;i<128;i++) err_str[i] =	bt816_mem_read8(RAM_ERR_REPORT+i);

			bt816_mem_write8(REG_CPURESET, 1);   /* hold co-processor engine in the reset condition */
			bt816_mem_write16(REG_CMD_READ, 0);  /* set REG_CMD_READ to 0 */
			bt816_mem_write16(REG_CMD_WRITE, 0); /* set REG_CMD_WRITE to 0 */
			bt816_mem_write32(REG_CMD_DL, 0);    /* reset REG_CMD_DL to 0 as required by the BT81x programming guide, should not hurt FT8xx */
			bt816_mem_write8(REG_CPURESET, 0);
		}

		uint16_t cur_key = get_pressed_keys();
		if(cur_key && (cur_key!=prev_key)) {
			key_cmd = cur_key;
		}else key_cmd = 0;
		prev_key = cur_key;

		if(stop_display==0) display_menu(key_cmd);
		else {
			if(stop_display>=100) stop_display-=100;
			else stop_display = 0;
		}


		//demo_display_fonts();
		vTaskDelay(100);
		check_new_records_update(100);
	}
}
