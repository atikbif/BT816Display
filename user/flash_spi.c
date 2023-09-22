/*
 * flas_spi.c
 *
 *  Created on: 2023 Sep 15
 *      Author: pelev
 */

#include "flash_spi.h"
#include "at32f435_437_board.h"

#define BUFFER_SIZE                      512

static spi_init_type spi_init_struct;
static uint8_t spi3_buf[BUFFER_SIZE];

void flash_spi_init() {
	gpio_init_type gpio_initstructure;
	spi_init_type spi_init_struct;

	crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	gpio_default_para_init(&gpio_initstructure);
	/* spi3 sck pin */
	gpio_initstructure.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
	gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_initstructure.gpio_pull           = GPIO_PULL_DOWN;
	gpio_initstructure.gpio_mode           = GPIO_MODE_MUX;
	gpio_initstructure.gpio_pins = GPIO_PINS_10;
	gpio_init(GPIOC, &gpio_initstructure);
	gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE10, GPIO_MUX_6);

	/* spi3 miso pin */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
	gpio_initstructure.gpio_pins           = GPIO_PINS_11;
	gpio_init(GPIOC, &gpio_initstructure);
	gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE11, GPIO_MUX_6);

	/* spi3 mosi pin */
	gpio_initstructure.gpio_pull           = GPIO_PULL_UP;
	gpio_initstructure.gpio_pins           = GPIO_PINS_12;
	gpio_init(GPIOC, &gpio_initstructure);
	gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE12, GPIO_MUX_6);

	crm_periph_clock_enable(CRM_SPI3_PERIPH_CLOCK, TRUE);
	spi_default_para_init(&spi_init_struct);
	spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
	spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
	spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
	spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
	spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
	spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
	spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
	spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
	spi_init(SPI3, &spi_init_struct);

	spi_enable(SPI3, TRUE);

	gpio_init_type gpio_init_struct;

	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	gpio_default_para_init(&gpio_init_struct);

	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_2;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOD, &gpio_init_struct);

	gpio_bits_set(GPIOD, GPIO_PINS_2);

	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	gpio_default_para_init(&gpio_init_struct);

	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_3;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOB, &gpio_init_struct);

	gpio_bits_set(GPIOB, GPIO_PINS_3);
}

uint8_t write_data(uint8_t *buf, uint16_t len, uint8_t ms) {
	uint16_t i=0;
	while(spi_i2s_flag_get(SPI3, SPI_I2S_RDBF_FLAG) != RESET) spi_i2s_data_receive(SPI3);
	while(i<len) {
		while(spi_i2s_flag_get(SPI3, SPI_I2S_TDBE_FLAG) == RESET);
		spi_i2s_data_transmit(SPI3, buf[i]);
		while(spi_i2s_flag_get(SPI3, SPI_I2S_RDBF_FLAG) == RESET);
		spi3_buf[i] = spi_i2s_data_receive(SPI3);
		i++;
	}
	return 1;
}

uint8_t read_data(uint8_t *buf, uint16_t len, uint8_t ms) {
	uint16_t i=0;
	while(spi_i2s_flag_get(SPI3, SPI_I2S_RDBF_FLAG) != RESET) spi_i2s_data_receive(SPI3);
	while(i<len) {
		while(spi_i2s_flag_get(SPI3, SPI_I2S_TDBE_FLAG) == RESET);
		spi_i2s_data_transmit(SPI3, 0xFF);
		while(spi_i2s_flag_get(SPI3, SPI_I2S_RDBF_FLAG) == RESET);
		buf[i] = spi_i2s_data_receive(SPI3);
		i++;
	}
	return 1;
}

void chip_select(enum FLASH_TYPE fl_type) {
	if(fl_type==AT45) {
		gpio_bits_reset(GPIOD, GPIO_PINS_2);
	}else if(fl_type==ZD25) {
		gpio_bits_reset(GPIOB, GPIO_PINS_3);
	}
}

void chip_deselect(enum FLASH_TYPE fl_type) {
	if(fl_type==AT45) {
		gpio_bits_set(GPIOD, GPIO_PINS_2);
	}else if(fl_type==ZD25) {
		gpio_bits_set(GPIOB, GPIO_PINS_3);
	}
}
