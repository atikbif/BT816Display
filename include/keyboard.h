/*
 * keyboard.h
 *
 *  Created on: 2023 Feb 8
 *      Author: pelev
 */

#ifndef INCLUDE_KEYBOARD_H_
#define INCLUDE_KEYBOARD_H_

#include <stdint.h>

void keyboard_task_function(void *pvParameters);
uint16_t get_pressed_keys();

#endif /* INCLUDE_KEYBOARD_H_ */
