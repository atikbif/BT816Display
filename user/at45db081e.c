/*
 * at45db081e.c
 *
 *  Created on: Mar 5, 2021
 *      Author: User
 */

#include "at45db081e.h"
#include "at32f435_437_board.h"
#include "flash_spi.h"

uint8_t at45_error = 0;

uint8_t at45_get_info(struct at_info *info) {
	uint8_t cmd = 0x9F;
	chip_select(AT45);
	delay_ms(100);
	if(write_data(&cmd, 1, 50)) {
		if(read_data((uint8_t*)info, sizeof(info), 50)) {
			chip_deselect(AT45);
			return 1;
		}
	}
	chip_deselect(AT45);
	return 0;
}

uint8_t at45_read_page(uint16_t page_num, uint8_t *buf) {
	if(page_num>4095) return 0;
	static uint8_t tx_buf[8] = {0,0,0,0,0,0,0,0};
	tx_buf[0] = 0xD2;
	tx_buf[1] = page_num >> 8;
	tx_buf[2] = page_num & 0xFF;
	tx_buf[3] = 0x00;
	chip_select(AT45);
	if(write_data(tx_buf, sizeof(tx_buf), 50)) {
		if(read_data(buf, 256, 50)) {
			chip_deselect(AT45);
			return 1;
		}
	}
	chip_deselect(AT45);
	return 0;
}

uint8_t at45_write_page(uint16_t page_num, uint8_t *buf) {
	if(page_num>4095) return 0;
	static uint8_t tx_buf[4] = {0,0,0,0};
	tx_buf[0] = 0x82;
	tx_buf[1] = page_num >> 8;
	tx_buf[2] = page_num & 0xFF;
	tx_buf[3] = 0x00;
	chip_select(AT45);
	if(write_data(tx_buf, sizeof(tx_buf), 50)) {
		if(write_data(buf, 256,50)) {
			chip_deselect(AT45);
			volatile uint16_t reg = 0;
			while(1) {
				if(at45_read_status_reg((uint16_t*)&reg)) {
					if((reg&=0x8080)==0x8080) {
						break;
					}
				}
			}
			return 1;
		}
	}
	chip_deselect(AT45);
	return 0;
}

uint8_t at45_read_status_reg(uint16_t *reg) {
	uint8_t cmd = 0xD7;
	chip_select(AT45);
	if(write_data(&cmd, 1, 50)) {
		if(read_data((uint8_t*)reg, 2, 50)) {
			chip_deselect(AT45);
			return 1;
		}
	}
	chip_deselect(AT45);
	return 0;
}

uint8_t at45_erase() {
	static uint8_t tx_buf[4] = {0xC7,0x94,0x80,0x9A};
	chip_select(AT45);
	if(write_data(tx_buf, sizeof(tx_buf), 50)) {
		chip_deselect(AT45);
		return 1;
	}
	chip_deselect(AT45);
	return 0;
}

uint8_t at45_set_page_256() {
	static uint8_t tx_buf[4] = {0x3D,0x2A,0x80,0xA6};
	chip_select(AT45);
	if(write_data(tx_buf, sizeof(tx_buf), 50)) {
		chip_deselect(AT45);
		return 1;
	}
	chip_deselect(AT45);
	return 0;
}

uint8_t at45_init() {
	uint8_t res = 0;
	at45_error = 0;
	struct at_info info;
	flash_spi_init();
	res = at45_get_info(&info);
	if(res) {
		if(info.manufacturer_id != 0x1F) at45_error = 1;
		if(info.dev_id1 != 0x25) at45_error = 1;
		if(info.dev_id2 != 0x00) at45_error = 1;
	}
	if(res && at45_error==0) at45_set_page_256();
	if(at45_error) return 0;
	return 1;
}
