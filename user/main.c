/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.6
  * @date     2022-03-11
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to 
  * download from Artery official website is the copyrighted work of Artery. 
  * Artery authorizes customers to use, copy, and distribute the BSP 
  * software and its related documentation for the purpose of design and 
  * development in conjunction with Artery microcontrollers. Use of the 
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"
#include "at32_emac.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "backlight.h"
#include "bt816_spi.h"
#include "display_task.h"
#include "keyboard.h"
#include "plc_data.h"
#include "can.h"
#include "cluster_state.h"
#include "cur_time.h"
#include "config.h"
#include "trend_data.h"
#include "cluster_state_menu.h"
#include "can_task.h"
#include "message_archive.h"
#include "at45db081e.h"
#include "fatfs.h"
#include "bt816_cmd.h"

/** @addtogroup UTILITIES_examples
  * @{
  */
  
/** @addtogroup FreeRTOS_demo
  * @{
  */

extern uint16_t can1_rx_tmr;
extern uint16_t can1_tx_tmr;
extern uint16_t hb_cnt;
extern uint8_t plc_can_link;
extern uint8_t get_io_names_flag;
extern cluster cl;

extern uint32_t cur_long_time;
extern cluster_info_data_type cluster_data;

extern volatile uint8_t fl_buf[4096];

extern void tcpip_stack_init(void);
extern void udpecho_init(void);
extern void tcpecho_init(void);

TaskHandle_t network_handler;
TaskHandle_t lcd_handler;
TaskHandle_t key_handler;
TaskHandle_t can_handler;

volatile uint8_t rst_flag = 0;
volatile uint16_t rst_tmr = 0;

void network_task_function(void *pvParameters);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

	system_clock_config();
	at32_board_init();
	delay_init();

	init_backlight();
	delay_ms(500);
	bt816_spi_init();
	init_plc_data();

	can1_init();
	init_cluster(&cl);
	init_trends();

	uint8_t try = 0;
	while(try<5) {
		if(bt816_init()) break;
		delay_ms(100);
		try++;
	}


	at32_led_init(LED_POW);
	at32_led_init(LED_ERR);

	if(gpio_input_data_bit_read(GPIOD, GPIO_PINS_1)==RESET) {
		delay_ms(500);
		MX_FATFS_Init();
		at45_init();
		FATFS fileSystem;
		FIL rdFile;
		UINT rdBytes;
		uint8_t nand_res = 0;
		delay_ms(100);

		uint8_t file_found_flag = 0;
		uint32_t led_cnt = 0;

		if(f_mount(&fileSystem, "0", 1) == FR_OK)
		{
			uint8_t path[] = "lcdconf.bin";
			FILINFO fInfo;
			nand_res = 0;
			if(f_stat((const char*)path, &fInfo) == FR_OK) {
				FRESULT res = f_open(&rdFile, (char*)path, FA_READ);
				if(res==FR_OK) {
					if(fInfo.fsize) {
						file_found_flag = 1;
						uint32_t wr_cnt = 0;
						while(wr_cnt<fInfo.fsize) {
							uint32_t remain_cnt = fInfo.fsize-wr_cnt;
							if(remain_cnt>=4096) {
								res = f_lseek(&rdFile, wr_cnt);
								if(res==FR_OK) {
									res = f_read(&rdFile, (void*)fl_buf, 4096, &rdBytes);
									if(res==FR_OK && rdBytes==4096) {
										// запись flash
										bt816_cmd_memwrite(0,4096,(uint8_t*)fl_buf);
										delay_ms(10);
										bt816_cmd_flashupdate(4096+wr_cnt,0,4096);
										delay_ms(100);
										wr_cnt+=4096;
										led_cnt++;
										if(led_cnt>=1) {
											led_cnt = 0;
											at32_led_toggle(LED_POW);
											at32_led_toggle(LED_ERR);
										}
									}else break;
								}else break;
							}else {
								res = f_lseek(&rdFile, wr_cnt);
								if(res==FR_OK) {
									res = f_read(&rdFile, (void*)fl_buf, remain_cnt, &rdBytes);
									if(res==FR_OK && rdBytes==remain_cnt) {
										uint32_t i = 0;
										while((remain_cnt+i)<4096) {
											fl_buf[remain_cnt+i] = 0;
											i++;
										}
										// запись flash
										bt816_cmd_memwrite(0,4096,(uint8_t*)fl_buf);
										delay_ms(10);
										bt816_cmd_flashupdate(4096+wr_cnt,0,4096);
										delay_ms(100);
										led_cnt++;
										if(led_cnt>=1) {
											led_cnt = 0;
											at32_led_toggle(LED_POW);
											at32_led_toggle(LED_ERR);
										}
										wr_cnt+=remain_cnt;
									}else break;
								}else break;
							}
						}
					}
				}
			}
		}
		if(file_found_flag==0) {
			for(uint8_t i=0;i<6;i++) {
				at32_led_toggle(LED_ERR);
				delay_ms(500);
			}
		}

		delay_ms(500);
	}

	at32_led_on(LED_POW);
	at32_led_on(LED_ERR);

	/* init usart1 */
	uart1_init(115200);
	uart2_init(115200);

	rs485_1_send_data("Start\r\n",7);
	rs485_2_send_data("Start\r\n",7);

	init_cur_time();

	/* enter critical */
	taskENTER_CRITICAL();

  
	if(xTaskCreate((TaskFunction_t )network_task_function,
				 (const char*    )"Network_task",
				 (uint16_t       )(1024*1),
				 (void*          )NULL,
				 (UBaseType_t    )2,
				 (TaskHandle_t*  )&network_handler) != pdPASS)
	{
		rs485_1_send_data("Network task create error\r\n",27);
		rs485_2_send_data("Network task create error\r\n",27);
	}
	else
	{
		rs485_1_send_data("Network task create OK\r\n",24);
		rs485_2_send_data("Network task create OK\r\n",24);
	}

	if(xTaskCreate((TaskFunction_t )lcd_task_function,
				 (const char*    )"LCD_task",
				 (uint16_t       )(1024*2),
				 (void*          )NULL,
				 (UBaseType_t    )2,
				 (TaskHandle_t*  )&lcd_handler) != pdPASS)
	{
		rs485_1_send_data("LCD task create error\r\n",23);
		rs485_2_send_data("LCD task create error\r\n",23);
	}else {
		rs485_1_send_data("LCD task create OK\r\n",20);
		rs485_2_send_data("LCD task create OK\r\n",20);
	}

	if(xTaskCreate((TaskFunction_t )keyboard_task_function,
				 (const char*    )"KEY_task",
				 (uint16_t       )(configMINIMAL_STACK_SIZE),
				 (void*          )NULL,
				 (UBaseType_t    )2,
				 (TaskHandle_t*  )&key_handler) != pdPASS)
	{
		rs485_1_send_data("KEY task create error\r\n",23);
		rs485_2_send_data("KEY task create error\r\n",23);
	}else {
		rs485_1_send_data("KEY task create OK\r\n",20);
		rs485_2_send_data("KEY task create OK\r\n",20);
	}
  
  	if(xTaskCreate((TaskFunction_t )can_task_function,
  				 (const char*    )"CAN_task",
  				 (uint16_t       )(configMINIMAL_STACK_SIZE),
  				 (void*          )NULL,
  				 (UBaseType_t    )2,
  				 (TaskHandle_t*  )&can_handler) != pdPASS)
	{
		rs485_1_send_data("CAN task create error\r\n",23);
		rs485_2_send_data("CAN task create error\r\n",23);
	}else {
		rs485_1_send_data("CAN task create OK\r\n",20);
		rs485_2_send_data("CAN task create OK\r\n",20);
	}

  /* exit critical */            
  taskEXIT_CRITICAL();      
              
  /* start scheduler */            
  vTaskStartScheduler(); 
  
}


void network_task_function(void *pvParameters)
{
	uint16_t cnt = 0;
	uint16_t calc_cnt = 0;
	while(emac_system_init() == ERROR);
	tcpip_stack_init();
	udpecho_init();
	tcpecho_init();
	while(1)
	{
		cnt++;
		if(cnt%900==0) {
			at32_led_toggle(LED_POW);
			cur_long_time = time_to_uint32();
		}
		if(get_io_names_flag) {
			// send can request
			get_io_names_flag++;
			if(get_io_names_flag>=10) get_io_names_flag = 0;
		}
		if(cnt%100==0) {
			plc_data_calculate();
		}
		can1_rx_tmr++;
		if(can1_rx_tmr>=20) {
			can1_rx_tmr = 0;
			gpio_bits_reset(GPIOG,GPIO_PINS_4);
		}
		can1_tx_tmr++;
		if(can1_tx_tmr>=20) {
			can1_tx_tmr = 0;
			gpio_bits_reset(GPIOG,GPIO_PINS_3);
		}
		hb_cnt++;
		if(hb_cnt>=3000) {
			if(plc_can_link) {
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
			hb_cnt = 0;
			plc_can_link = 0;
			cluster_data.plc_link[0] = 0;
			reset_plc_data();
		}
		if(rst_flag) {
			if(rst_tmr) rst_tmr--;
			if(rst_tmr==0) NVIC_SystemReset();
		}
		vTaskDelay(1);

	}
}

