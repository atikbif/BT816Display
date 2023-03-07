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
void draw_mnemo();
uint8_t read_mnemo_data(uint16_t mnemo_num);

#endif /* INCLUDE_MNEMO_H_ */
