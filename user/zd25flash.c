/*
 * zd25flash.c
 *
 *  Created on: 2023 Sep 15
 *      Author: pelev
 */


#include "zd25flash.h"
#include "flash_spi.h"
#include "at32f435_437_board.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t zd25_err = 0;

uint16_t zd25_flash_buf[ZD25_SECTOR_SIZE / 2];

uint8_t zd25_check_device() {
	uint8_t res = 0;
	uint8_t cmd_buf[] = {0x90,0x00,0x00,0x00};
	uint8_t answer_buf[] = {0x00,0x00};
	chip_select(ZD25);
	vTaskDelay(1);
	if(write_data(cmd_buf, sizeof(cmd_buf), 50)) {
		if(read_data(answer_buf, sizeof(answer_buf), 50)) {
			if((answer_buf[0]==0xBA) && (answer_buf[1]==0x16)) res = 1;
		}
	}
	chip_deselect(ZD25);
	return res;
}

uint8_t zd25_init() {
	flash_spi_init();
	return zd25_check_device();
}

void zd25_write_enable() {
	uint8_t cmd_buf[] = {0x06};
	chip_select(ZD25);
	vTaskDelay(1);
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	chip_deselect(ZD25);
}

void zd25_write_disable() {
	uint8_t cmd_buf[] = {0x04};
	chip_select(ZD25);
	vTaskDelay(1);
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	chip_deselect(ZD25);
}

uint16_t zd25_read_status_reg() {
	uint16_t res = 0;
	uint8_t cmd_buf[] = {0x05};
	uint8_t answer_buf[] = {0x00,0x00};
	chip_select(ZD25);
	vTaskDelay(1);
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	read_data(answer_buf, sizeof(answer_buf), 50);
	res = ((uint16_t)answer_buf[0]<<8) | answer_buf[1];
	chip_deselect(ZD25);
	return res;
}

void zd25_read_data(uint32_t addr, uint16_t cnt, uint8_t *buf) {
	uint8_t cmd_buf[] = {0x03,0x00,0x00,0x00};
	chip_select(ZD25);
	vTaskDelay(1);
	cmd_buf[1] = (addr>>16)&0xFF;
	cmd_buf[2] = (addr>>8)&0xFF;
	cmd_buf[3] = addr&0xFF;
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	read_data(buf, cnt, 50);
	chip_deselect(ZD25);
}

void zd25_write_data(uint32_t addr, uint16_t cnt, uint8_t *buf) {
	uint8_t cmd_buf[] = {0x02,0x00,0x00,0x00};
	chip_select(ZD25);
	vTaskDelay(1);
	cmd_buf[1] = (addr>>16)&0xFF;
	cmd_buf[2] = (addr>>8)&0xFF;
	cmd_buf[3] = addr&0xFF;
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	write_data(buf, cnt, 50);
	chip_deselect(ZD25);
}

void zd25_sector_erase(uint32_t addr) {
	uint8_t cmd_buf[] = {0x20,0x00,0x00,0x00};
	chip_select(ZD25);
	vTaskDelay(1);
	cmd_buf[1] = (addr>>16)&0xFF;
	cmd_buf[2] = (addr>>8)&0xFF;
	cmd_buf[3] = addr&0xFF;
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	chip_deselect(ZD25);
}

void zd25_power_down() {
	uint8_t cmd_buf[] = {0xB9};
	chip_select(ZD25);
	vTaskDelay(1);
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	chip_deselect(ZD25);
}

void zd25_release_power_down() {
	uint8_t cmd_buf[] = {0xAB};
	chip_select(ZD25);
	vTaskDelay(1);
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	chip_deselect(ZD25);
}

void zd25_enable_reset() {
	uint8_t cmd_buf[] = {0x66};
	chip_select(ZD25);
	vTaskDelay(1);
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	chip_deselect(ZD25);
}

void zd25_reset() {
	uint8_t cmd_buf[] = {0x99};
	chip_select(ZD25);
	vTaskDelay(1);
	write_data(cmd_buf, sizeof(cmd_buf), 50);
	chip_deselect(ZD25);
}

uint8_t zd25_get_busy() {
	uint16_t reg = zd25_read_status_reg();
	if(reg&0x01) return 1;
	return 0;
}

void zd25_flash_read(uint32_t read_addr, uint16_t *p_buffer, uint16_t num_read) {
	zd25_read_data(read_addr, num_read*2, (uint8_t*)p_buffer);
}

error_status zd25_flash_write_nocheck(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write) {
	zd25_write_enable();
	zd25_write_data(write_addr, num_write*2, (uint8_t*)p_buffer);
	for(uint16_t i=0;i<50;i++) {
		if(zd25_get_busy()==0) return SUCCESS;
		vTaskDelay(1);
	}
	return ERROR;
}

error_status zd25_flash_write(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write) {
	uint32_t sector_position;
	uint16_t sector_offset;
	uint16_t sector_remain;
	uint16_t i;
	flash_status_type status = FLASH_OPERATE_DONE;

	sector_position = write_addr / ZD25_SECTOR_SIZE;
	sector_offset = (write_addr % ZD25_SECTOR_SIZE) / 2;
	sector_remain = ZD25_SECTOR_SIZE / 2 - sector_offset;
	if(num_write <= sector_remain) sector_remain = num_write;
	while(1)
	{
		zd25_flash_read(sector_position * ZD25_SECTOR_SIZE, zd25_flash_buf, ZD25_SECTOR_SIZE / 2);
		for(i = 0; i < sector_remain; i++)
		{
			if(zd25_flash_buf[sector_offset + i] != 0xFFFF) break;
		}
		if(i < sector_remain)
		{
			zd25_write_enable();
			zd25_sector_erase(sector_position * ZD25_SECTOR_SIZE);
			for(uint16_t j=0;j<50;j++) {
				if(zd25_get_busy()==0) break;
				vTaskDelay(1);
			}

			zd25_flash_read(sector_position * ZD25_SECTOR_SIZE, zd25_flash_buf, ZD25_SECTOR_SIZE / 2);

			for(i = 0; i < sector_remain; i++)
			{
				zd25_flash_buf[i + sector_offset] = p_buffer[i];
			}
			if(zd25_flash_write_nocheck(sector_position * ZD25_SECTOR_SIZE, zd25_flash_buf, ZD25_SECTOR_SIZE / 2) != SUCCESS) return ERROR;
		}
		else
		{
			if(zd25_flash_write_nocheck(write_addr, p_buffer, sector_remain) != SUCCESS)	return ERROR;
		}
		if(num_write == sector_remain)	break;
		else
		{
			sector_position++;
			sector_offset = 0;
			p_buffer += sector_remain;
			write_addr += (sector_remain * 2);
			num_write -= sector_remain;
			if(num_write > (ZD25_SECTOR_SIZE / 2)) sector_remain = ZD25_SECTOR_SIZE / 2;
			else sector_remain = num_write;
		}
	}
	return SUCCESS;
}
