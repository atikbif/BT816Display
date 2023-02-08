/**
  **************************************************************************
  * @file     at32f435_437_board.c
  * @version  v2.0.6
  * @date     2022-03-11
  * @brief    header file for at-start board. set of firmware functions to 
  *           manage leds and push-button. initialize delay function.
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

#ifndef __AT32F435_437_BOARD_H
#define __AT32F435_437_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "at32f435_437.h"

/** @addtogroup AT32F435_437_board
  * @{
  */

/** @addtogroup BOARD
  * @{
  */

/** @defgroup BOARD_pins_definition 
  * @{
  */

/**
  * this header include define support list:
  *   1. at-start-f435 v1.x board
  *   2. at-start-f437 v1.x board
  * if define AT_START_F435_V1, the header file support at-start-f437 v1.x board
  * if define AT_START_F437_V1, the header file support at-start-f437 v1.x board
  */

#if !defined (AT_START_F435_V1) && !defined (AT_START_F437_V1)
#error "please select first the board at-start device used in your application (in at32f435_437_board.h file)"
#endif

/******************** define led ********************/
typedef enum
{
  LED_POW                                   = 0,
  LED_ERR                                   = 1,
}led_type;

#define LED_NUM                          2

#define LED_POW_PIN                     GPIO_PINS_12
#define LED_POW_GPIO                    GPIOD
#define LED_POW_GPIO_CRM_CLK            CRM_GPIOD_PERIPH_CLOCK

#define LED_ERR_PIN                     GPIO_PINS_2
#define LED_ERR_GPIO                    GPIOG
#define LED_ERR_GPIO_CRM_CLK            CRM_GPIOG_PERIPH_CLOCK

#define BT_PD_PIN							GPIO_PINS_14
#define BT_PD_GPIO                        	GPIOD
#define BT_PD_GPIO_CRM_CLK                	CRM_GPIOD_PERIPH_CLOCK

/**************** define print uart ******************/

#define RS485_1_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK
#define RS485_1_UART_TX_PIN                GPIO_PINS_9
#define RS485_1_UART_TX_GPIO               GPIOA
#define RS485_1_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK
#define RS485_1_UART_TX_PIN_SOURCE         GPIO_PINS_SOURCE9
#define RS485_1_UART_TX_PIN_MUX_NUM        GPIO_MUX_7

#define RS485_1_UART_DE_PIN                GPIO_PINS_12
#define RS485_1_UART_DE_GPIO               GPIOA
#define RS485_1_UART_DE_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK
#define RS485_1_UART_DE_PIN_SOURCE         GPIO_PINS_SOURCE12
#define RS485_1_UART_DE_PIN_MUX_NUM        GPIO_MUX_7

#define RS485_2_UART_CRM_CLK               CRM_USART2_PERIPH_CLOCK
#define RS485_2_UART_TX_PIN                GPIO_PINS_5
#define RS485_2_UART_TX_GPIO               GPIOD
#define RS485_2_UART_TX_GPIO_CRM_CLK       CRM_GPIOD_PERIPH_CLOCK
#define RS485_2_UART_TX_PIN_SOURCE         GPIO_PINS_SOURCE5
#define RS485_2_UART_TX_PIN_MUX_NUM        GPIO_MUX_7

#define RS485_2_UART_DE_PIN                GPIO_PINS_4
#define RS485_2_UART_DE_GPIO               GPIOD
#define RS485_2_UART_DE_GPIO_CRM_CLK       CRM_GPIOD_PERIPH_CLOCK
#define RS485_2_UART_DE_PIN_SOURCE         GPIO_PINS_SOURCE4
#define RS485_2_UART_DE_PIN_MUX_NUM        GPIO_MUX_7

/**
  * @}
  */

/** @defgroup BOARD_exported_functions
  * @{
  */

/******************** functions ********************/
void at32_board_init(void);

/* led operation function */
void at32_led_init(led_type led);
void at32_led_on(led_type led);
void at32_led_off(led_type led);
void at32_led_toggle(led_type led);

/* delay function */
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_sec(uint16_t sec);

/* printf uart init function */
void uart1_init(uint32_t baudrate);
void uart2_init(uint32_t baudrate);

void rs485_1_send_data(uint8_t* buf, uint8_t cnt);
void rs485_2_send_data(uint8_t* buf, uint8_t cnt);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

