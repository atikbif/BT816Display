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

/** @addtogroup UTILITIES_examples
  * @{
  */
  
/** @addtogroup FreeRTOS_demo
  * @{
  */
extern void tcpip_stack_init(void);
extern void udpecho_init(void);
extern void tcpecho_init(void);

TaskHandle_t network_handler;

/* led3 task */
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
  
  /* init led2*/
  at32_led_init(LED2);
  /* init led3*/
  at32_led_init(LED3);
  /* init led4*/
  at32_led_init(LED4);
  
  /* init usart1 */
  uart_print_init(115200);
  
  /* enter critical */
  taskENTER_CRITICAL(); 

  
  if(xTaskCreate((TaskFunction_t )network_task_function,     
                 (const char*    )"Network_task",   
                 (uint16_t       )(1024*1),
                 (void*          )NULL,
                 (UBaseType_t    )2,
                 (TaskHandle_t*  )&network_handler) != pdPASS)
  {
    printf("Network task could not be created as there was insufficient heap memory remaining.\r\n");
  }        
  else
  {
    printf("Network task was created successfully.\r\n");
  }
  
  /* exit critical */            
  taskEXIT_CRITICAL();      
              
  /* start scheduler */            
  vTaskStartScheduler(); 
  
}


/* led3 task function */
void network_task_function(void *pvParameters)
{
	while(emac_system_init() == ERROR);
	tcpip_stack_init();
	udpecho_init();
	tcpecho_init();
	while(1)
	{
		//at32_led_toggle(LED3);
		vTaskDelay(500);
	}
}

/**
  * @}
  */ 

/**
  * @}
  */ 
