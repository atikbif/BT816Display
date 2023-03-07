/*
 * lamps.h
 *
 *  Created on: 2023 Mar 2
 *      Author: pelev
 */

#ifndef INCLUDE_LAMPS_H_
#define INCLUDE_LAMPS_H_

#include <stdint.h>

enum LAMP_COL {LAMP_GRAY, LAMP_RED, LAMP_BLUE, LAMP_YELLOW, LAMP_GREEN};

const uint8_t* get_lamp_picture_data(enum LAMP_COL col);
uint16_t get_lamp_picture_size(enum LAMP_COL col);

#endif /* INCLUDE_LAMPS_H_ */
