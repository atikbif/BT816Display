/*
 * bt816_spi.c
 *
 *  Created on: 2023 Jan 17
 *      Author: Roman
 */

#include "bt816_spi.h"
#include "at32f435_437_board.h"


void bt816_spi_init(void) {
	gpio_init_type gpio_init_struct;

	/* enable the qspi clock */
	crm_periph_clock_enable(CRM_QSPI1_PERIPH_CLOCK, TRUE);

	/* enable the pin clock */
	crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOH_PERIPH_CLOCK, TRUE);

	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);

	/* configure the io0 gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = GPIO_PINS_9;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOC, &gpio_init_struct);
	gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE9, GPIO_MUX_9);

	/* configure the io1 gpio */
	gpio_init_struct.gpio_pins = GPIO_PINS_3;
	gpio_init(GPIOH, &gpio_init_struct);
	gpio_pin_mux_config(GPIOH, GPIO_PINS_SOURCE3, GPIO_MUX_10);

	/* configure the sck gpio */
	gpio_init_struct.gpio_pins = GPIO_PINS_3;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE3, GPIO_MUX_9);


	/* switch to cmd port */
	qspi_xip_enable(QSPI1, FALSE);

	/* set sclk */
	qspi_clk_division_set(QSPI1, QSPI_CLK_DIV_12);

	/* set sck idle mode 0 */
	qspi_sck_mode_set(QSPI1, QSPI_SCK_MODE_0);

	/* set wip in bit 0 */
	qspi_busy_config(QSPI1, QSPI_BUSY_OFFSET_0);

	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_6;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOG, &gpio_init_struct);
	gpio_bits_write(GPIOG,GPIO_PINS_6,TRUE);
}

void bt816_cs_clear(void)
{
	gpio_bits_write(GPIOG,GPIO_PINS_6,TRUE);
}

void bt816_cs_set(void)
{
	gpio_bits_write(GPIOG,GPIO_PINS_6,FALSE);
}

void bt816_spi_transmit_array(uint32_t cnt, const uint8_t *ptr) {
	if(cnt) {
		cnt--;
		qspi_cmd_type cmd_config;
		cmd_config.pe_mode_enable = FALSE;
		cmd_config.pe_mode_operate_code = 0;
		cmd_config.instruction_code = *ptr++;
		cmd_config.instruction_length = QSPI_CMD_INSLEN_1_BYTE;
		cmd_config.address_code = 0;
		cmd_config.address_length = QSPI_CMD_ADRLEN_0_BYTE;
		cmd_config.data_counter = cnt;
		cmd_config.second_dummy_cycle_num = 0;
		cmd_config.operation_mode = QSPI_OPERATE_MODE_111;
		cmd_config.read_status_config = QSPI_RSTSC_SW_ONCE;
		cmd_config.read_status_enable = FALSE;
		cmd_config.write_data_enable = TRUE;
		qspi_cmd_operation_kick(QSPI1, &cmd_config);
		while(cnt) {
			while(qspi_flag_get(QSPI1, QSPI_TXFIFORDY_FLAG) == RESET);
			qspi_byte_write(QSPI1, *ptr++);
			cnt--;
		}
		while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
		qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
	}
}

void bt816_spi_transmit_array_with_padding(uint32_t cnt, const uint8_t *ptr) {
	if(cnt) {
		uint32_t length = (cnt + 3U) & (~3U);
		length--;
		cnt--;
		qspi_cmd_type cmd_config;
		cmd_config.pe_mode_enable = FALSE;
		cmd_config.pe_mode_operate_code = 0;
		cmd_config.instruction_code = *ptr++;
		cmd_config.instruction_length = QSPI_CMD_INSLEN_1_BYTE;
		cmd_config.address_code = 0;
		cmd_config.address_length = QSPI_CMD_ADRLEN_0_BYTE;
		cmd_config.data_counter = length;
		cmd_config.second_dummy_cycle_num = 0;
		cmd_config.operation_mode = QSPI_OPERATE_MODE_111;
		cmd_config.read_status_config = QSPI_RSTSC_SW_ONCE;
		cmd_config.read_status_enable = FALSE;
		cmd_config.write_data_enable = TRUE;
		qspi_cmd_operation_kick(QSPI1, &cmd_config);
		uint32_t i = 0;
		while(i<cnt) {
			while(qspi_flag_get(QSPI1, QSPI_TXFIFORDY_FLAG) == RESET);
			qspi_byte_write(QSPI1, *ptr++);
			i++;
		}
		for(uint32_t i = cnt;i<length;i++) {
			qspi_byte_write(QSPI1,0U);
		}

		while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
		qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
	}
}

void bt816_spi_transmit(uint8_t data) {
	qspi_cmd_type cmd_config;

	cmd_config.pe_mode_enable = FALSE;
	cmd_config.pe_mode_operate_code = 0;
	cmd_config.instruction_code = data;
	cmd_config.instruction_length = QSPI_CMD_INSLEN_1_BYTE;
	cmd_config.address_code = 0;
	cmd_config.address_length = QSPI_CMD_ADRLEN_0_BYTE;
	cmd_config.data_counter = 0;
	cmd_config.second_dummy_cycle_num = 0;
	cmd_config.operation_mode = QSPI_OPERATE_MODE_111;
	cmd_config.read_status_config = QSPI_RSTSC_SW_ONCE;
	cmd_config.read_status_enable = FALSE;
	cmd_config.write_data_enable = TRUE;
	qspi_cmd_operation_kick(QSPI1, &cmd_config);
	while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
	qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}


void bt816_spi_transmit_32(uint32_t value) {
	bt816_spi_transmit(value & 0xFF);
	bt816_spi_transmit((value>>8) & 0xFF);
	bt816_spi_transmit((value>>16) & 0xFF);
	bt816_spi_transmit((value>>24) & 0xFF);
}

void bt816_spi_send_cmd_and_receive_array(uint8_t cmd, uint32_t cnt, uint8_t *rx_buf) {
	uint8_t res = 0;
	qspi_cmd_type cmd_config;
	cmd_config.pe_mode_enable = FALSE;
	cmd_config.pe_mode_operate_code = 0;
	cmd_config.instruction_code = cmd;
	cmd_config.instruction_length = QSPI_CMD_INSLEN_1_BYTE;
	cmd_config.address_code = 0;
	cmd_config.address_length = QSPI_CMD_ADRLEN_0_BYTE;
	cmd_config.data_counter = cnt;
	cmd_config.second_dummy_cycle_num = 0;
	cmd_config.operation_mode = QSPI_OPERATE_MODE_111;//144;
	cmd_config.read_status_config = QSPI_RSTSC_HW_AUTO;
	cmd_config.read_status_enable = FALSE;
	cmd_config.write_data_enable = FALSE;

	qspi_cmd_operation_kick(QSPI1, &cmd_config);
	uint8_t len = 0;
	do {
		if(cnt >= (32*4)) // FIFO DEPTH
		{
			len = 32*4;
		}else {
			len = cnt;
		}
		while(qspi_flag_get(QSPI1, QSPI_RXFIFORDY_FLAG) == RESET);
		for(uint8_t i=0; i<len; ++i)
		{
			*rx_buf++ = qspi_byte_read(QSPI1);
		}
		cnt -= len;
	}while(cnt);

	 /* wait command completed */
	 while(qspi_flag_get(QSPI1, QSPI_CMDSTS_FLAG) == RESET);
	 qspi_flag_clear(QSPI1, QSPI_CMDSTS_FLAG);
}

