/*
 * fonts.h
 *
 *  Created on: 2023 Mar 2
 *      Author: pelev
 */

#ifndef INCLUDE_FONTS_H_
#define INCLUDE_FONTS_H_

#include <stdint.h>

enum CYR_FONT {CYR_FONT14, CYR_FONT22, CYR_FONT30, CYR_FONT40};

const uint8_t* get_xfont_data(enum CYR_FONT font);
uint32_t get_xfont_size(enum CYR_FONT font);
const uint8_t* get_glyph_data(enum CYR_FONT font);
uint32_t get_glyph_size(enum CYR_FONT font);

#endif /* INCLUDE_FONTS_H_ */
