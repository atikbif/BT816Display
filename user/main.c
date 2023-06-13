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

extern void tcpip_stack_init(void);
extern void udpecho_init(void);
extern void tcpecho_init(void);

TaskHandle_t network_handler;
TaskHandle_t lcd_handler;
TaskHandle_t key_handler;

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
  
  at32_led_init(LED_POW);
  at32_led_init(LED_ERR);

  
  /* init usart1 */
  uart1_init(115200);
  uart2_init(115200);
  
  rs485_1_send_data("Start\r\n",7);
  rs485_2_send_data("Start\r\n",7);



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
				 (uint16_t       )(1024*1),
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
			//imitate_plc_data();
			at32_led_toggle(LED_POW);
			send_heartbeat();
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
			hb_cnt = 0;
			plc_can_link = 0;
			cluster_data.plc_link[0] = 0;
		}
		//at32_led_toggle(LED_POW);
		vTaskDelay(1);

	}
}

