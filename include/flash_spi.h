/*
 * flash_spi.h
 *
 *  Created on: 2023 Sep 15
 *      Author: pelev
 */

#ifndef INCLUDE_FLASH_SPI_H_
#define INCLUDE_FLASH_SPI_H_

#include <stdint.h>

enum FLASH_TYPE {AT45,ZD25};

void flash_spi_init();
uint8_t write_data(uint8_t *buf, uint16_t len, uint8_t ms);
uint8_t read_data(uint8_t *buf, uint16_t len, uint8_t ms);
void chip_select(enum FLASH_TYPE fl_type);
void chip_deselect(enum FLASH_TYPE fl_type);

#endif /* INCLUDE_FLASH_SPI_H_ */
