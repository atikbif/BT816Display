/*
 * demo_display.h
 *
 *  Created on: 2023 Feb 8
 *      Author: pelev
 */

#ifndef INCLUDE_DEMO_DISPLAY_H_
#define INCLUDE_DEMO_DISPLAY_H_

#include <stdio.h>

#define RAM_TOP_POINT	(0xF5800)

#define MEM_L1GLYPH14 	(RAM_TOP_POINT - 6*1024u)
#define MEM_L1GLYPH22 	(MEM_L1GLYPH14 - 10*1024u)
#define MEM_L1GLYPH30 	(MEM_L1GLYPH22 - 20*1024u)
#define MEM_L1GLYPH40 	(MEM_L1GLYPH30 - 34*1024u)
#define MEM_L1FONT14 (MEM_L1GLYPH40 - 1024u)
#define MEM_L1FONT22 (MEM_L1FONT14 - 1024u)
#define MEM_L1FONT30 (MEM_L1FONT22 - 1024u)
#define MEM_L1FONT40 (MEM_L1FONT30 - 1024u)
#define MEM_LAMP1	(MEM_L1FONT40 - 5*1024u)
#define MEM_LAMP2	(MEM_LAMP1 - 5*1024u)
#define MEM_LAMP3	(MEM_LAMP2 - 5*1024u)
#define MEM_LAMP4	(MEM_LAMP3 - 5*1024u)
#define MEM_LAMP5	(MEM_LAMP4 - 5*1024u)

void init_display_fonts(void);
void demo_display_fonts(void);
void display_value(uint16_t value);
void demo_display1(void);

#endif /* INCLUDE_DEMO_DISPLAY_H_ */
