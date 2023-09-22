/*
 * zd25flash.h
 *
 *  Created on: 2023 Sep 15
 *      Author: pelev
 */

#ifndef INCLUDE_ZD25FLASH_H_
#define INCLUDE_ZD25FLASH_H_

#include <stdint.h>
#include "at32f435_437_board.h"

#define ZD25_ACRHIVE_START_ADDR		0x000000
#define ZD25_ARCHIVE_PAGE_CNT		128
#define ZD25_SECTOR_SIZE			4096

uint8_t zd25_init();
uint8_t zd25_check_device();
void zd25_write_enable();
void zd25_write_disable();
uint16_t zd25_read_status_reg();
void zd25_read_data(uint32_t addr, uint16_t cnt, uint8_t *buf);
void zd25_write_data(uint32_t addr, uint16_t cnt, uint8_t *buf);
void zd25_sector_erase(uint32_t addr);
void zd25_power_down();
void zd25_release_power_down();
void zd25_enable_reset();
void zd25_reset();
uint8_t zd25_get_busy();

void zd25_flash_read(uint32_t read_addr, uint16_t *p_buffer, uint16_t num_read);
error_status zd25_flash_write_nocheck(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write);
error_status zd25_flash_write(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write);

#endif /* INCLUDE_ZD25FLASH_H_ */
