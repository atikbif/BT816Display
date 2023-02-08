/**
  **************************************************************************
  * @file     at32f435_437_board.c
  * @version  v2.0.6
  * @date     2022-03-11
  * @brief    set of firmware functions to manage leds and push-button.
  *           initialize delay function.
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

/** @addtogroup AT32F435_437_board
  * @{
  */

/** @defgroup BOARD
  * @brief onboard periph driver
  * @{
  */

/* delay macros */
#define STEP_DELAY_MS                    50

/* at-start led resouce array */
gpio_type *led_gpio_port[LED_NUM]        = {LED_POW_GPIO, LED_ERR_GPIO};
uint16_t led_gpio_pin[LED_NUM]           = {LED_POW_PIN, LED_ERR_PIN};
crm_periph_clock_type led_gpio_crm_clk[LED_NUM] = {LED_POW_GPIO_CRM_CLK, LED_ERR_GPIO_CRM_CLK};

/* delay variable */
static __IO uint32_t fac_us;
static __IO uint32_t fac_ms;


void rs485_1_send_data(uint8_t* buf, uint8_t cnt)
{
  while(cnt--){
    while(usart_flag_get(USART1, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART1, *buf++);
  }
}

void rs485_2_send_data(uint8_t* buf, uint8_t cnt)
{
  while(cnt--){
    while(usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART2, *buf++);
  }
}

/**
  * @brief  initialize uart
  * @param  baudrate: uart baudrate
  * @retval none
  */
void uart1_init(uint32_t baudrate)
{
	gpio_init_type gpio_init_struct;

	/* enable the uart and gpio clock */
	crm_periph_clock_enable(RS485_1_UART_CRM_CLK, TRUE);
	crm_periph_clock_enable(RS485_1_UART_TX_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(RS485_1_UART_DE_GPIO_CRM_CLK, TRUE);

	gpio_default_para_init(&gpio_init_struct);

	/* configure the uart tx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = RS485_1_UART_TX_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(RS485_1_UART_TX_GPIO, &gpio_init_struct);

	gpio_pin_mux_config(RS485_1_UART_TX_GPIO, RS485_1_UART_TX_PIN_SOURCE, RS485_1_UART_TX_PIN_MUX_NUM);

	gpio_init_struct.gpio_pins = RS485_1_UART_DE_PIN;
	gpio_init(RS485_1_UART_DE_GPIO, &gpio_init_struct);

	gpio_pin_mux_config(RS485_1_UART_DE_GPIO, RS485_1_UART_DE_PIN_SOURCE, RS485_1_UART_DE_PIN_MUX_NUM);

	/* configure uart param */
	usart_init(USART1, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
	usart_rs485_delay_time_config(USART1, 2, 2);
	usart_de_polarity_set(USART1, USART_DE_POLARITY_HIGH);
	usart_rs485_mode_enable(USART1, TRUE);
	usart_transmitter_enable(USART1, TRUE);
	usart_enable(USART1, TRUE);
}

void uart2_init(uint32_t baudrate)
{
	gpio_init_type gpio_init_struct;

	crm_periph_clock_enable(RS485_2_UART_CRM_CLK, TRUE);
	crm_periph_clock_enable(RS485_2_UART_TX_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(RS485_2_UART_DE_GPIO_CRM_CLK, TRUE);

    gpio_default_para_init(&gpio_init_struct);

    /* configure the uart tx pin */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = RS485_2_UART_TX_PIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(RS485_2_UART_TX_GPIO, &gpio_init_struct);

    gpio_pin_mux_config(RS485_2_UART_TX_GPIO, RS485_2_UART_TX_PIN_SOURCE, RS485_2_UART_TX_PIN_MUX_NUM);

    gpio_init_struct.gpio_pins = RS485_2_UART_DE_PIN;
    gpio_init(RS485_2_UART_DE_GPIO, &gpio_init_struct);

    gpio_pin_mux_config(RS485_2_UART_DE_GPIO, RS485_2_UART_DE_PIN_SOURCE, RS485_2_UART_DE_PIN_MUX_NUM);

    /* configure uart param */
    usart_init(USART2, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_rs485_delay_time_config(USART2, 2, 2);
    usart_de_polarity_set(USART2, USART_DE_POLARITY_HIGH);
    usart_rs485_mode_enable(USART2, TRUE);
    usart_transmitter_enable(USART2, TRUE);
    usart_enable(USART2, TRUE);
}

/**
  * @brief  board initialize interface init led and button
  * @param  none
  * @retval none
  */
void at32_board_init()
{
  crm_periph_clock_enable(BT_PD_GPIO_CRM_CLK, TRUE);

  	/* set default parameter */
  	gpio_init_type gpio_init_struct;
  	gpio_default_para_init(&gpio_init_struct);

  	/* configure the led gpio */
  	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  	gpio_init_struct.gpio_pins = BT_PD_PIN;
  	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  	gpio_init(BT_PD_GPIO, &gpio_init_struct);

  	gpio_bits_reset(BT_PD_GPIO, BT_PD_PIN);

}

/**
  * @brief  configure led gpio   
  * @param  led: specifies the led to be configured.
  * @retval none
  */
void at32_led_init(led_type led)
{
  gpio_init_type gpio_init_struct;

  /* enable the led clock */
  crm_periph_clock_enable(led_gpio_crm_clk[led], TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure the led gpio */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = led_gpio_pin[led];
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(led_gpio_port[led], &gpio_init_struct);
}

/**
  * @brief  turns selected led on.
  * @param  led: specifies the led to be set on. 
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval none
  */
void at32_led_on(led_type led)
{
  if(led > (LED_NUM - 1))
    return;
  if(led_gpio_pin[led])
    led_gpio_port[led]->clr = led_gpio_pin[led];
}

/**
  * @brief  turns selected led off.
  * @param  led: specifies the led to be set off. 
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4 
  * @retval none
  */
void at32_led_off(led_type led)
{
  if(led > (LED_NUM - 1))
    return;
  if(led_gpio_pin[led])
    led_gpio_port[led]->scr = led_gpio_pin[led];
}

/**
  * @brief  turns selected led tooggle.
  * @param  led: specifies the led to be set off. 
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4 
  * @retval none
  */
void at32_led_toggle(led_type led)
{
  if(led > (LED_NUM - 1))
    return;
  if(led_gpio_pin[led])
    led_gpio_port[led]->odt ^= led_gpio_pin[led];
}

/**
  * @brief  initialize delay function   
  * @param  none
  * @retval none
  */
void delay_init()
{
  /* configure systick */
  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
  fac_us = system_core_clock / (1000000U);
  fac_ms = fac_us * (1000U);
}

/**
  * @brief  inserts a delay time.
  * @param  nus: specifies the delay time length, in microsecond.
  * @retval none
  */
void delay_us(uint32_t nus)
{
  uint32_t temp = 0;
  SysTick->LOAD = (uint32_t)(nus * fac_us);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
  do
  {
    temp = SysTick->CTRL;
  }while((temp & 0x01) && !(temp & (1 << 16)));

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
}

/**
  * @brief  inserts a delay time.
  * @param  nms: specifies the delay time length, in milliseconds.
  * @retval none
  */
void delay_ms(uint16_t nms)
{
  uint32_t temp = 0;
  while(nms)
  {
    if(nms > STEP_DELAY_MS)
    {
      SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * fac_ms);
      nms -= STEP_DELAY_MS;
    }
    else
    {
      SysTick->LOAD = (uint32_t)(nms * fac_ms);
      nms = 0;
    }
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
      temp = SysTick->CTRL;
    }while((temp & 0x01) && !(temp & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
  }
}

/**
  * @brief  inserts a delay time.
  * @param  sec: specifies the delay time, in seconds.
  * @retval none
  */
void delay_sec(uint16_t sec)
{
  uint16_t index;
  for(index = 0; index < sec; index++)
  {
    delay_ms(500);
    delay_ms(500);
  }
}

/**
  * @}
  */

/**
  * @}
  */

