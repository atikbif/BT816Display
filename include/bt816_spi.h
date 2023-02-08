/*
 * bt816_spi.h
 *
 *  Created on: 2023 Jan 17
 *      Author: Roman
 */

#ifndef INCLUDE_BT816_SPI_H_
#define INCLUDE_BT816_SPI_H_

#include <stdint.h>

void bt816_spi_init(void);
void bt816_cs_clear(void);
void bt816_cs_set(void);
void bt816_spi_transmit(uint8_t data);
void bt816_spi_transmit_32(uint32_t value);
void bt816_spi_transmit_array(uint32_t cnt, const uint8_t *ptr);
void bt816_spi_transmit_array_with_padding(uint32_t cnt, const uint8_t *ptr);

void bt816_spi_send_cmd_and_receive_array(uint8_t cmd, uint32_t cnt, uint8_t *rx_buf);

#endif /* INCLUDE_BT816_SPI_H_ */
