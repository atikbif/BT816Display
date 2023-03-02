/*
 * mnemo.h
 *
 *  Created on: 2023 Feb 28
 *      Author: pelev
 */

#ifndef INCLUDE_MNEMO_H_
#define INCLUDE_MNEMO_H_

#include <stdint.h>

void init_mnemo();
uint16_t mnemo_read_cnt_from_config(uint32_t conf_addr);
void draw_mnemo(uint32_t conf_addr, uint16_t mnemo_num);

#endif /* INCLUDE_MNEMO_H_ */
