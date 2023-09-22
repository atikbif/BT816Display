/*
 * at45db081e.h
 *
 *  Created on: Mar 5, 2021
 *      Author: User
 */

#ifndef INC_AT45DB081E_H_
#define INC_AT45DB081E_H_

#include <stdint.h>

struct at_info{
	uint8_t manufacturer_id;
	uint8_t dev_id1;
	uint8_t dev_id2;
	uint8_t ext_dev_info1;
	uint8_t ext_dev_info2;
};

uint8_t at45_get_info(struct at_info *info);
uint8_t at45_read_page(uint16_t page_num, uint8_t *buf);
uint8_t at45_write_page(uint16_t page_num, uint8_t *buf);
uint8_t at45_read_status_reg(uint16_t *reg);
uint8_t at45_erase();
uint8_t at45_set_page_256();
uint8_t at45_init();

#endif /* INC_AT45DB081E_H_ */
