/*
 * bt816_cmd.c
 *
 *  Created on: 2022 Sep 13
 *      Author: Roman
 */

#include "bt816_cmd.h"
#include "at32f435_437_board.h"
#include "bt816_spi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "demo_display.h"

#define WR_BLOB	0

static const uint8_t unified_blob [4096] = {
	112, 223, 251, 146, 30, 1, 0, 0, 232, 166, 48, 0, 24, 1, 16, 0, 0, 0, 248, 218, 74, 167, 42, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 64, 128, 141, 114, 98, 114, 91, 54, 104, 64, 148, 50, 241, 51, 251, 51, 135, 51, 56, 54, 126, 63, 128, 114, 36, 144, 0, 219, 112, 192, 0, 204, 128, 128, 39, 64, 160, 255, 36, 144, 0, 219, 112, 192, 0, 204,
	0, 128, 17, 192, 20, 128, 3, 209, 145, 118, 64, 128, 3, 194, 17, 199, 150, 255, 16, 64, 162, 63, 199, 128, 135, 114, 81, 116, 130, 50, 104, 64, 81, 52, 137, 63, 251, 51, 135, 51, 129, 54, 117, 54, 26, 0, 0, 0, 49, 70, 24, 35, 132, 58, 227, 107, 43, 157, 255, 255, 255, 255, 255, 255, 70, 114, 197, 49, 104, 64, 148, 50, 241, 51, 251, 51, 135, 51, 11, 0,
	181, 63, 235, 115, 200, 128, 0, 199, 202, 255, 1, 128, 188, 172, 3, 206, 49, 128, 135, 114, 2, 128, 217, 113, 102, 54, 157, 128, 0, 199, 211, 255, 1, 128, 188, 172, 3, 206, 64, 128, 141, 114, 102, 54, 194, 128, 0, 199, 218, 255, 1, 128, 4, 191, 3, 206, 102, 54, 32, 128, 0, 199, 230, 255, 2, 114, 186, 128, 188, 128, 48, 64, 230, 255, 1, 128, 72, 191, 3, 206,
	102, 54, 239, 128, 0, 199, 254, 255, 2, 114, 113, 128, 3, 199, 254, 255, 1, 128, 110, 191, 3, 206, 89, 114, 1, 128, 86, 191, 3, 206, 212, 115, 208, 115, 98, 114, 183, 128, 135, 114, 98, 114, 32, 128, 224, 115, 130, 114, 23, 54, 176, 54, 190, 63, 160, 11, 89, 241, 135, 9, 121, 64, 241, 47, 145, 140, 4, 233, 98, 55, 50, 134, 16, 250, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 3, 0, 88, 1, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 119, 118, 84, 50, 16, 254, 220, 186, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90,
	90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90
};

void bt816_power_down_on() {
	gpio_bits_reset(BT_PD_GPIO, BT_PD_PIN);
}

void bt816_power_down_off() {
	gpio_bits_set(BT_PD_GPIO, BT_PD_PIN);
}

uint32_t bt816_alpha_func(uint8_t func, uint8_t ref) {
	uint32_t res = 0x09000000;
	res |= ((uint32_t)(func&0x07)) << 8;
	res |= ref;
	return res;
}

uint32_t bt816_begin(uint8_t prim) {
	uint32_t res = 0x1F000000;
	return res | prim;
}

uint32_t bt816_bitmap_ext_format(uint16_t format) {
	uint32_t res = 0x2E000000;
	return res | format;
}

uint32_t bt816_bitmap_handle(uint8_t handle) {
	uint32_t res = 0x05000000;
	return res | (handle & 0x1F);
}

uint32_t bt816_bitmap_layout(uint8_t format, uint16_t linestride, uint16_t height) {
	uint32_t res = 0x07000000;
	res |= ((uint32_t)(format & 0x1F))<<19;
	res |= ((uint32_t)(linestride & 0x3FF))<<9;
	res |= (height & 0x1FF);
	return res;
}

uint32_t bt816_bitmap_layout_h(uint8_t linestride, uint8_t height) {
	uint32_t res = 0x28000000;
	res |= ((uint32_t)(linestride & 0x03))<<2;
	res |= (height & 0x03);
	return res;
}

uint32_t bt816_bitmap_size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height) {
	uint32_t res = 0x08000000;
	if(filter) res |= ((uint32_t)1)<<20;
	if(wrapx) res |= ((uint32_t)1)<<19;
	if(wrapy) res |= ((uint32_t)1)<<18;
	res |= ((uint32_t)(width & 0x1FF))<<9;
	res |= (height & 0x1FF);
	return res;
}

uint32_t bt816_bitmap_size_h(uint8_t width, uint8_t height) {
	uint32_t res = 0x29000000;
	res |= ((uint32_t)(width & 0x03))<<2;
	res |= (height & 0x03);
	return res;
}

uint32_t bt816_bitmap_source(uint32_t addr) {
	uint32_t res = 0x01000000;
	res |= (uint32_t)(addr & 0xFFFFFF);
	return res;
}

uint32_t bt816_bitmap_transform_a(uint8_t p, uint32_t v) {
	uint32_t res = 0x15000000;
	if(p) res |= ((uint32_t)1)<<17;
	res |= v & 0x1FFFF;
	return res;
}

uint32_t bt816_bitmap_transform_b(uint8_t p, uint32_t v) {
	uint32_t res = 0x16000000;
	if(p) res |= ((uint32_t)1)<<17;
	res |= v & 0x1FFFF;
	return res;
}

uint32_t bt816_bitmap_transform_c(uint32_t v) {
	uint32_t res = 0x17000000;
	res |= v & 0xFFFFFF;
	return res;
}

uint32_t bt816_bitmap_transform_d(uint8_t p, uint32_t v) {
	uint32_t res = 0x18000000;
	if(p) res |= ((uint32_t)1)<<17;
	res |= v & 0x1FFFF;
	return res;
}

uint32_t bt816_bitmap_transform_e(uint8_t p, uint32_t v) {
	uint32_t res = 0x19000000;
	if(p) res |= ((uint32_t)1)<<17;
	res |= v & 0x1FFFF;
	return res;
}

uint32_t bt816_bitmap_transform_f(uint32_t v) {
	uint32_t res = 0x1A000000;
	res |= v & 0xFFFFFF;
	return res;
}

uint32_t bt816_blend_func(uint8_t src, uint8_t dst) {
	uint32_t res = 0x0B000000;
	res |= ((uint32_t)(src & 0x07))<<3;
	res |= (dst & 0x07);
	return res;
}

uint32_t bt816_call(uint16_t dest) {
	uint32_t res = 0x1D000000;
	res |= dest;
	return res;
}

uint32_t bt816_cell(uint8_t cell) {
	uint32_t res = 0x06000000;
	res |= cell & 0x7F;
	return res;
}

uint32_t bt816_clear(uint8_t color_buf, uint8_t stencil_buf, uint8_t tag_buf) {
	uint32_t res = 0x26000000;
	if(tag_buf) res|=0x01;
	if(stencil_buf) res |= 0x02;
	if(color_buf) res |= 0x04;
	return res;
}

uint32_t bt816_clear_color_a(uint8_t alpha) {
	uint32_t res = 0x26000000;
	res |= alpha;
	return res;
}

uint32_t bt816_clear_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t res = 0x02000000;
	res |= (uint32_t)r<<16;
	res |= (uint32_t)b<<8;
	res |= g;
	return res;
}

uint32_t bt816_clear_stencil(uint8_t s) {
	uint32_t res = 0x11000000;
	res |= s;
	return res;
}

uint32_t bt816_clear_tag(uint8_t t) {
	uint32_t res = 0x12000000;
	res |= t;
	return res;
}

uint32_t bt816_color_a(uint8_t alpha) {
	uint32_t res = 0x10000000;
	res |= alpha;
	return res;
}

uint32_t bt816_color_mask(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	uint32_t res = 0x20000000;
	if(r) res |= 0x08;
	if(g) res |= 0x04;
	if(b) res |= 0x02;
	if(a) res |= 0x01;
	return res;
}

uint32_t bt816_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t res = 0x04000000;
	res |= (uint32_t)r<<16;
	res |= (uint32_t)b<<8;
	res |= g;
	return res;
}

uint32_t bt816_display() {
	return 0;
}

uint32_t bt816_end() {
	return 0x21000000;
}

uint32_t bt816_jump(uint16_t dest) {
	uint32_t res = 0x1E000000;
	res |= dest;
	return res;
}

uint32_t bt816_line_width(uint16_t width) {
	uint32_t res = 0x0E000000;
	res |= width & 0x0FFF;
	return res;
}

uint32_t bt816_macro(uint8_t m) {
	uint32_t res = 0x25000000;
	if(m) res |= 0x01;
	return res;
}

uint32_t bt816_nop() {
	uint32_t res = 0x2D000000;
	return res;
}

uint32_t bt816_palette_source(uint32_t addr) {
	uint32_t res = 0x2A000000;
	res |= addr & 0x3FFFFF;
	return res;
}

uint32_t bt816_point_size(uint16_t size) {
	uint32_t res = 0x0D000000;
	return res |= size & 0x1FFF;
}

uint32_t bt816_restore_context() {
	uint32_t res = 0x23000000;
	return res;
}

uint32_t bt816_return() {
	uint32_t res = 0x24000000;
	return res;
}

uint32_t bt_save_context() {
	uint32_t res = 0x22000000;
	return res;
}

uint32_t bt816_scissor_size(uint16_t width, uint16_t height) {
	uint32_t res = 0x1C000000;
	res |= ((uint32_t)(width & 0x0FFF)) << 12;
	res |= (uint32_t)(height & 0x0FFF);
	return res;
}

uint32_t bt816_scissor_xy(uint16_t x, uint16_t y) {
	uint32_t res = 0x1B000000;
	res |= ((uint32_t)(x & 0x07FF)) << 11;
	res |= (uint32_t)(y & 0x07FF);
	return res;
}

uint32_t bt816_stencil_func(uint8_t func, uint8_t ref, uint8_t mask) {
	uint32_t res = 0x0A000000;
	res |= ((uint32_t)(func & 0x0F)) << 16;
	res |= ((uint32_t)ref) << 8;
	res |= mask;
	return res;
}

uint32_t bt816_stencil_mask(uint8_t mask) {
	uint32_t res = 0x13000000;
	res |= mask;
	return res;
}

uint32_t bt816_stencil_op(uint8_t sfall, uint8_t spass) {
	uint32_t res = 0x0C000000;
	res |= ((uint32_t)(sfall & 0x07)) << 3;
	res |= spass & 0x07;
	return res;
}

uint32_t bt816_tag(uint8_t s) {
	uint32_t res = 0x03000000;
	res |= s;
	return res;
}

uint32_t bt816_tag_mask(uint8_t mask) {
	uint32_t res = 0x14000000;
	if(mask) res |= 0x01;
	return res;
}

uint32_t bt816_vertex2f(uint16_t x, uint16_t y) {
	uint32_t res = 0x40000000;
	res |= ((uint32_t)(x&0x7FFF)) << 15;
	res |= ((uint32_t)(y&0x7FFF));
	return res;
}

uint32_t bt816_vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell) {
	uint32_t res = 0x80000000;
	res |= ((uint32_t)(x&0x1FF)) << 21;
	res |= ((uint32_t)(y&0x1FF)) << 12;
	res |= ((uint32_t)(handle&0x1F)) << 7;
	res |= cell&0x7F;
	return res;
}

uint32_t bt816_vertex_format(uint8_t frac) {
	uint32_t res = 0x27000000;
	res |= frac&0x07;
	return res;
}

uint32_t bt816_vertex_translate_x(uint16_t x) {
	uint32_t res = 0x2B000000;
	res |= x;
	return res;
}

uint32_t bt816_vertex_translate_y(uint16_t y) {
	uint32_t res = 0x2C000000;
	res |= y;
	return res;
}

static void bt816_cmd_send(uint8_t cmd, uint8_t parameter) {
	bt816_cs_set();
	bt816_spi_transmit(cmd);
	bt816_spi_transmit(parameter);
	bt816_spi_transmit(0);
	bt816_cs_clear();
}

uint8_t bt816_mem_read8(uint32_t addr)
{
    uint8_t res = 0;
    bt816_cs_set();
    uint8_t rx_buf[2];
	bt816_spi_transmit((addr>>16)&0xFF);
	bt816_spi_transmit((addr>>8)&0xFF);
	bt816_spi_send_cmd_and_receive_array(addr&0xFF, 2, rx_buf);
	res = rx_buf[1];
	bt816_cs_clear();
    return res; /* return byte read */
}

uint16_t bt816_mem_read16(uint32_t addr)
{
    uint16_t res = 0;

    bt816_cs_set();
    uint8_t rx_buf[3];
	bt816_spi_transmit((addr>>16)&0xFF);
	bt816_spi_transmit((addr>>8)&0xFF);
	bt816_spi_send_cmd_and_receive_array(addr&0xFF, 3, rx_buf);
	res = rx_buf[1];
	res |= ((uint16_t)rx_buf[2])<<8;
	bt816_cs_clear();
    return res; /* return byte read */
}

uint32_t bt816_mem_read32(uint32_t addr)
{
	uint32_t res = 0;
	bt816_cs_set();
	uint8_t rx_buf[5];
	bt816_spi_transmit((addr>>16)&0xFF);
	bt816_spi_transmit((addr>>8)&0xFF);
	bt816_spi_send_cmd_and_receive_array(addr&0xFF, 5, rx_buf);
	res = rx_buf[1];
	res |= ((uint16_t)rx_buf[2])<<8;
	res |= ((uint16_t)rx_buf[3])<<16;
	res |= ((uint16_t)rx_buf[4])<<24;
	bt816_cs_clear();
	return res; /* return byte read */
}

void bt816_mem_write8(uint32_t addr, uint8_t value)
{
	bt816_cs_set();

	addr |= 0x00800000;

	bt816_spi_transmit((addr>>16)&0xFF);
	bt816_spi_transmit((addr>>8)&0xFF);
	bt816_spi_transmit(addr&0xFF);
	bt816_spi_transmit(value);
	bt816_cs_clear();
}

void bt816_mem_write16(uint32_t addr, uint16_t value)
{
	bt816_cs_set();
	addr |= 0x00800000;
	bt816_spi_transmit((addr>>16)&0xFF);
	bt816_spi_transmit((addr>>8)&0xFF);
	bt816_spi_transmit(addr&0xFF);
	bt816_spi_transmit(value&0xFF);
	bt816_spi_transmit(value>>8);
	bt816_cs_clear();
}

void bt816_mem_write32(uint32_t addr, uint32_t value)
{
	bt816_cs_set();

	addr |= 0x00800000;

	bt816_spi_transmit((addr>>16)&0xFF);
	bt816_spi_transmit((addr>>8)&0xFF);
	bt816_spi_transmit(addr&0xFF);

	bt816_spi_transmit(value&0xFF);
	bt816_spi_transmit((value>>8)&0xFF);
	bt816_spi_transmit((value>>16)&0xFF);
	bt816_spi_transmit((value>>24)&0xFF);
	bt816_cs_clear();
}

void bt816_mem_write_buffer(uint32_t bt_addr, const uint8_t *data, uint32_t cnt) {
	bt816_cs_set();
	bt816_spi_transmit((uint8_t)(bt_addr >> 16U) | 0x80);
	bt816_spi_transmit((uint8_t)(bt_addr >> 8U));
	bt816_spi_transmit((uint8_t)(bt_addr & 0xFF));

	for (uint32_t i = 0U; i < cnt; i++)
	{
		bt816_spi_transmit(data[i]);
	}

	uint32_t length = (cnt + 3U) & (~3U);

	for(uint32_t i = cnt;i<length;i++) {
		bt816_spi_transmit(0U);
	}

	bt816_cs_clear();
}

void bt816_block_transfer(const uint8_t *p_data, uint32_t len)
{
    uint32_t bytes_left;

    bytes_left = len;
    while (bytes_left > 0U)
    {
        uint32_t block_len;

        block_len = (bytes_left > 3840UL) ? 3840UL : bytes_left;

        bt816_cs_set();
        bt816_spi_transmit((uint8_t) 0xB0U); /* high-byte of REG_CMDB_WRITE + MEM_WRITE */
        bt816_spi_transmit((uint8_t) 0x25U); /* middle-byte of REG_CMDB_WRITE */
        bt816_spi_transmit((uint8_t) 0x78U); /* low-byte of REG_CMDB_WRITE */
        bt816_spi_transmit_array_with_padding(block_len, p_data);
        bt816_cs_clear();
        p_data = &p_data[block_len];
        bytes_left -= block_len;
    }
}

static void bt816_begin_cmd(uint32_t command)
{
	uint32_t addr = REG_CMDB_WRITE | 0x00800000;

	bt816_spi_transmit((addr>>16)&0xFF);
	bt816_spi_transmit((addr>>8)&0xFF);
	bt816_spi_transmit(addr&0xFF);

	bt816_spi_transmit(command&0xFF);
	bt816_spi_transmit((command>>8)&0xFF);
	bt816_spi_transmit((command>>16)&0xFF);
	bt816_spi_transmit((command>>24)&0xFF);
}

void bt816_cmd_dl(uint32_t command) {
	bt816_cs_set();
	bt816_begin_cmd(command);
	bt816_cs_clear();
}

uint32_t bt816_cmd_regread(uint32_t ptr) {
	uint32_t res = 0;
	bt816_cs_set();
	bt816_begin_cmd(CMD_REGREAD);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(0);
	bt816_cs_clear();
	uint16_t cmdoffset = bt816_mem_read16(REG_CMD_WRITE); /* read the co-processor write pointer */
    cmdoffset -= 4U;
    cmdoffset &= 0x0fffU;
    return (bt816_mem_read32(RAM_CMD + cmdoffset));
}

void bt816_cmd_dlstart() {
	bt816_cmd_dl(CMD_DLSTART);
}

void bt816_cmd_coldstart() {
	bt816_cmd_dl(CMD_COLDSTART);
}

void bt816_cmd_swap() {
	bt816_cmd_dl(CMD_SWAP);
}

void bt816_cmd_append(uint32_t ptr, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_REGREAD);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

void bt816_cmd_flashattach() {
	bt816_cmd_dl(CMD_FLASHATTACH);
}

void bt816_cmd_flashdetach() {
	bt816_cmd_dl(CMD_FLASHDETACH);
}

void bt816_cmd_flasherase() {
	bt816_cmd_dl(CMD_FLASHERASE);
}

uint32_t bt816_cmd_flashfast() {
	uint16_t cmdoffset;
	bt816_cs_set();
	bt816_begin_cmd(CMD_FLASHFAST);
	bt816_spi_transmit_32(0UL);
	bt816_cs_clear();

	cmdoffset = bt816_mem_read16(REG_CMD_WRITE); /* read the co-processor write pointer */
	cmdoffset -= 4U;
	cmdoffset &= 0x0fffU;
	uint32_t res = bt816_mem_read32(RAM_CMD + cmdoffset);
	return res;
}


void bt816_cmd_flashupdate (uint32_t dest, uint32_t src, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FLASHUPDATE);
	bt816_spi_transmit_32(dest);
	bt816_spi_transmit_32(src);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

void bt816_cmd_flashread(uint32_t dest, uint32_t src, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FLASHREAD);
	bt816_spi_transmit_32(dest);
	bt816_spi_transmit_32(src);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

void bt816_cmd_flashspidesel() {
	bt816_cmd_dl(CMD_FLASHSPIDESEL);
}

void bt816_cmd_flashsource(uint32_t ptr) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FLASHSOURCE);
	bt816_spi_transmit_32(ptr);
	bt816_cs_clear();
}

void bt816_cmd_flashspirx(uint32_t dest, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FLASHSPIRX);
	bt816_spi_transmit_32(dest);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

void bt816_cmd_flashspitx(uint32_t num, const uint8_t *p_data) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FLASHSPITX);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
	bt816_block_transfer(p_data, num);
}

// note: ptr must be 256 byte aligned, num must be a multiple of 256
void bt816_cmd_flashwrite(uint32_t ptr, uint32_t num, const uint8_t *p_data) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FLASHWRITE);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
    if (p_data != NULL)
    {
    	bt816_block_transfer(p_data, num);
    }
}

void bt816_cmd_inflate2(uint32_t ptr, uint32_t options, const uint8_t *p_data, uint32_t len) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_INFLATE2);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(options);
	bt816_cs_clear();

    if (0UL == options) /* direct data, not by Media-FIFO or Flash */
    {
        if (p_data != NULL)
        {
        	bt816_block_transfer(p_data, len);
        }
    }
}

/* get the properties of an image after a CMD_LOADIMAGE operation and write the values to the variables that are supplied by pointers*/
void bt816_cmd_getprops(uint32_t *p_pointer, uint32_t *p_width, uint32_t *p_height) {
    uint16_t cmdoffset;
    bt816_cs_set();
    bt816_begin_cmd(CMD_GETPROPS);
    bt816_spi_transmit_32(0UL);
    bt816_spi_transmit_32(0UL);
    bt816_spi_transmit_32(0UL);
    bt816_cs_clear();

    cmdoffset = bt816_mem_read16(REG_CMD_WRITE);

    if (p_pointer != NULL)
    {
        *p_pointer = bt816_mem_read32(RAM_CMD + ((cmdoffset - 12U) & 0xfffU));
    }
    if (p_width != NULL)
    {
        *p_width = bt816_mem_read32(RAM_CMD + ((cmdoffset - 8U) & 0xfffU));
    }
    if (p_height != NULL)
    {
        *p_height = bt816_mem_read32(RAM_CMD + ((cmdoffset - 4U) & 0xfffU));
    }
}

uint32_t bt816_cmd_getptr() {
    uint16_t cmdoffset;

    bt816_cs_set();
	bt816_begin_cmd(CMD_GETPTR);
	bt816_spi_transmit_32(0UL);
	bt816_cs_clear();

    cmdoffset = bt816_mem_read16(REG_CMD_WRITE); /* read the co-processor write pointer */
    cmdoffset -= 4U;
    cmdoffset &= 0x0fffU;
    return (bt816_mem_read32(RAM_CMD + cmdoffset));
}

void bt816_cmd_inflate(uint32_t ptr, const uint8_t *p_data, uint32_t len)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_INFLATE);
	bt816_spi_transmit_32(ptr);
	bt816_cs_clear();
    if (p_data != NULL)
    {
        bt816_block_transfer(p_data, len);
    }
}

void bt816_cmd_interrupt(uint32_t msec)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_INTERRUPT);
	bt816_spi_transmit_32(msec);
	bt816_cs_clear();
}

void bt816_cmd_loadimage(uint32_t ptr, uint32_t options, const uint8_t *p_data, uint32_t len) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_LOADIMAGE);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(options);
	bt816_cs_clear();

	/* direct data, neither by Media-FIFO or from Flash */
    if (((options & OPT_MEDIAFIFO) == 0UL) &&  ((options & OPT_FLASH) == 0UL)) {
        if (p_data != NULL) {
        	bt816_block_transfer(p_data, len);
        }
    }
}

void bt816_cmd_mediafifo(uint32_t ptr, uint32_t size) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_MEDIAFIFO);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(size);
	bt816_cs_clear();
}

void bt816_cmd_memcpy(uint32_t dest, uint32_t src, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_MEMCPY);
	bt816_spi_transmit_32(dest);
	bt816_spi_transmit_32(src);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

uint32_t bt816_cmd_memcrc(uint32_t ptr, uint32_t num) {
    uint16_t cmdoffset;
    bt816_cs_set();
    bt816_begin_cmd(CMD_MEMCRC);
    bt816_spi_transmit_32(ptr);
    bt816_spi_transmit_32(num);
    bt816_spi_transmit_32(0UL);
    bt816_cs_clear();
    cmdoffset = bt816_mem_read16(REG_CMD_WRITE); /* read the co-processor write pointer */
    cmdoffset -= 4U;
    cmdoffset &= 0x0fffU;
    return (bt816_mem_read32(RAM_CMD + cmdoffset));
}

void bt816_cmd_memset(uint32_t ptr, uint8_t value, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_MEMSET);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32((uint32_t)value);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

void bt816_cmd_memwrite(uint32_t mem_addr, uint32_t num, const uint8_t *data_ptr) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_MEMWRITE);
	bt816_spi_transmit_32(mem_addr);
	bt816_spi_transmit_32(num);
	uint32_t i = 0;
	while(i<num) {
		bt816_spi_transmit(*data_ptr++);
		i++;
	}
	uint32_t padding = num & 3U; /* 0, 1, 2 or 3 */
	padding = 4U - padding;   /* 4, 3, 2 or 1 */

	while (padding > 0U)
	{
		bt816_spi_transmit(0U);
		padding--;
	}
	bt816_cs_clear();
}

void bt816_cmd_memzero(uint32_t ptr, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_MEMZERO);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

void bt816_cmd_playvideo(uint32_t options, const uint8_t *p_data, uint32_t len) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_PLAYVIDEO);
	bt816_spi_transmit_32(options);
	bt816_cs_clear();

    if (((options & OPT_MEDIAFIFO) == 0UL) && ((options & OPT_FLASH) == 0UL)) {
        if (p_data != NULL)
        {
            bt816_block_transfer(p_data, len);
        }
    }
}

void bt816_cmd_setrotate(uint32_t rotation) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SETROTATE);
	bt816_spi_transmit_32(rotation);
	bt816_cs_clear();
}

void bt816_cmd_snapshot(uint32_t ptr) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SNAPSHOT);
	bt816_spi_transmit_32(ptr);
	bt816_cs_clear();
}

void bt816_cmd_snapshot2(uint32_t fmt, uint32_t ptr, int16_t xc0, int16_t yc0, int16_t width, int16_t height) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SNAPSHOT2);
	bt816_spi_transmit_32(fmt);
	bt816_spi_transmit_32(ptr);

	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));

	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) height));
	bt816_spi_transmit((uint8_t) (((uint16_t) height) >> 8U));
	bt816_cs_clear();
}

void bt816_cmd_track(int16_t xc0, int16_t yc0, int16_t width, int16_t height, int16_t tag) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_TRACK);

	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));

	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) height));
	bt816_spi_transmit((uint8_t) (((uint16_t) height) >> 8U));

	bt816_spi_transmit((uint8_t) ((uint16_t) tag));
	bt816_spi_transmit((uint8_t) (((uint16_t) tag) >> 8U));
	bt816_spi_transmit(0U);
	bt816_spi_transmit(0U);

	bt816_cs_clear();
}

void bt816_cmd_videoframe(uint32_t dest, uint32_t result_ptr) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_VIDEOFRAME);
	bt816_spi_transmit_32(dest);
	bt816_spi_transmit_32(result_ptr);
	bt816_cs_clear();
}

static void private_string_write(const char *p_text) {
    /* treat the array as bunch of bytes */
    const uint8_t *p_bytes = (const uint8_t *)p_text;

	uint8_t textindex = 0U;
	uint8_t padding;

	/* either leave on Zero or when the string is too long */
	while ((textindex < 249U) && (p_bytes[textindex] != 0U))
	{
		bt816_spi_transmit(p_bytes[textindex]);
		textindex++;
	}


	/* transmit at least one 0x00 byte */
	/* and up to four if the string happens to be 4-byte aligned already */
	padding = textindex & 3U; /* 0, 1, 2 or 3 */
	padding = 4U - padding;   /* 4, 3, 2 or 1 */

	while (padding > 0U)
	{
		bt816_spi_transmit(0U);
		padding--;
	}
}

void bt816_cmd_animdraw(int32_t chnl) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ANIMDRAW);
	bt816_spi_transmit_32((uint32_t) chnl);
	bt816_cs_clear();
}

void bt816_cmd_animframe(int16_t xc0, int16_t yc0, uint32_t aoptr, uint32_t frame) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ANIMFRAME);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit_32(aoptr);
	bt816_spi_transmit_32(frame);
	bt816_cs_clear();
}

void bt816_cmd_animstart(int32_t chnl, uint32_t aoptr, uint32_t loop) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ANIMSTART);
	bt816_spi_transmit_32((uint32_t) chnl);
	bt816_spi_transmit_32(aoptr);
	bt816_spi_transmit_32(loop);
	bt816_cs_clear();
}

void bt816_cmd_animstop(int32_t chnl) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ANIMSTOP);
	bt816_spi_transmit_32((uint32_t) chnl);
	bt816_cs_clear();
}

void bt816_cmd_animxy(int32_t chnl, int16_t xc0, int16_t yc0) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ANIMXY);
	bt816_spi_transmit_32((uint32_t) chnl);
	bt816_spi_transmit_32((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit_32((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit_32((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit_32((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_cs_clear();
}

void bt816_cmd_appendf(uint32_t ptr, uint32_t num) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_APPENDF);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(num);
	bt816_cs_clear();
}

uint16_t bt816_cmd_bitmap_transform(int32_t xc0, int32_t yc0, int32_t xc1,
                                int32_t yc1, int32_t xc2, int32_t yc2,
                                int32_t tx0, int32_t ty0, int32_t tx1,
                                int32_t ty1, int32_t tx2, int32_t ty2) {
    uint16_t ret_val = 0U;
	uint16_t cmdoffset;
	bt816_cs_set();
	bt816_begin_cmd(CMD_BITMAP_TRANSFORM);
	bt816_spi_transmit_32((uint32_t) xc0);
	bt816_spi_transmit_32((uint32_t) yc0);
	bt816_spi_transmit_32((uint32_t) xc1);
	bt816_spi_transmit_32((uint32_t) yc1);
	bt816_spi_transmit_32((uint32_t) xc2);
	bt816_spi_transmit_32((uint32_t) yc2);
	bt816_spi_transmit_32((uint32_t) tx0);
	bt816_spi_transmit_32((uint32_t) ty0);
	bt816_spi_transmit_32((uint32_t) tx1);
	bt816_spi_transmit_32((uint32_t) ty1);
	bt816_spi_transmit_32((uint32_t) tx2);
	bt816_spi_transmit_32((uint32_t) ty2);
	bt816_spi_transmit_32(0UL);
	bt816_cs_clear();
	cmdoffset = bt816_mem_read16(REG_CMD_WRITE);
	cmdoffset -= 4U;
	cmdoffset &= 0x0fffU;
	ret_val = (uint16_t) bt816_mem_read32(RAM_CMD + cmdoffset);
    return ret_val;
}

void bt816_cmd_fillwidth(uint32_t pixel) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FILLWIDTH);
	bt816_spi_transmit_32(pixel);
	bt816_cs_clear();
}

void bt816_cmd_gradienta(int16_t xc0, int16_t yc0, uint32_t argb0, int16_t xc1,
                        int16_t yc1, uint32_t argb1) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_GRADIENTA);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit_32(argb0);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc1));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc1) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc1));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc1) >> 8U));
	bt816_spi_transmit_32(argb1);
	bt816_cs_clear();
}

void bt816_cmd_rotatearound(int32_t xc0, int32_t yc0, int32_t angle, int32_t scale) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ROTATEAROUND);
	bt816_spi_transmit_32((uint32_t) xc0);
	bt816_spi_transmit_32((uint32_t) yc0);
	bt816_spi_transmit_32((uint32_t) angle);
	bt816_spi_transmit_32((uint32_t) scale);
	bt816_cs_clear();
}

/* "num_args" is the number of elements provided in "p_arguments[]" */
void bt816_cmd_button_var(int16_t xc0, int16_t yc0, int16_t width, int16_t height,
                        int16_t font, uint16_t options, const char *p_text,
                        uint8_t num_args, const uint32_t p_arguments[])
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_BUTTON);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) height));
	bt816_spi_transmit((uint8_t) (((uint16_t) height) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	private_string_write(p_text);

	if ((options & OPT_FORMAT) != 0U)
	{
		if (p_arguments != NULL)
		{
			for (uint8_t counter = 0U; counter < num_args; counter++)
			{
				bt816_spi_transmit_32(p_arguments[counter]);
			}
		}
	}
	bt816_cs_clear();
}

/* "num_args" is the number of elements provided in "p_arguments[]" */
void bt816_cmd_text_var(int16_t xc0, int16_t yc0, int16_t font,
                        uint16_t options, const char *p_text,
                        uint8_t num_args, const uint32_t p_arguments[])
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_TEXT);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	private_string_write(p_text);

	if ((options & OPT_FORMAT) != 0U)
	{
		if (p_arguments != NULL)
		{
			for (uint8_t counter = 0U; counter < num_args; counter++)
			{
				bt816_spi_transmit_32(p_arguments[counter]);
			}
		}
	}
	bt816_cs_clear();
}

void bt816_cmd_toggle_var(int16_t xc0, int16_t yc0, int16_t width, int16_t font,
                        uint16_t options, uint16_t state, const char *p_text,
                        uint8_t num_args, const uint32_t p_arguments[])
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_TOGGLE);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	bt816_spi_transmit((uint8_t) (state));
	bt816_spi_transmit((uint8_t) (state >> 8U));
	private_string_write(p_text);

	if ((options & OPT_FORMAT) != 0U)
	{
		if (p_arguments != NULL)
		{
			for (uint8_t counter = 0U; counter < num_args; counter++)
			{
				bt816_spi_transmit_32(p_arguments[counter]);
			}
		}
	}
	bt816_cs_clear();
}

void bt816_cmd_bgcolor(uint32_t color) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_BGCOLOR);
	bt816_spi_transmit((uint8_t) (color));
	bt816_spi_transmit((uint8_t) (color >> 8U));
	bt816_spi_transmit((uint8_t) (color >> 16U));
	bt816_spi_transmit(0U);
	bt816_cs_clear();
}

void bt816_cmd_button(int16_t xc0, int16_t yc0, int16_t width, int16_t height,
                    int16_t font, uint16_t options, const char *p_text)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_BUTTON);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) height));
	bt816_spi_transmit((uint8_t) (((uint16_t) height) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	private_string_write(p_text);
	bt816_cs_clear();
}

void bt816_cmd_clock(int16_t xc0, int16_t yc0, int16_t rad, uint16_t options, uint16_t hours,
                    uint16_t mins, uint16_t secs, uint16_t msecs)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_CLOCK);
	bt816_spi_transmit((uint8_t)((uint16_t) xc0));
	bt816_spi_transmit((uint8_t)(((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t)((uint16_t) yc0));
	bt816_spi_transmit((uint8_t)(((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t)((uint16_t) rad));
	bt816_spi_transmit((uint8_t)(((uint16_t) rad) >> 8U));
	bt816_spi_transmit((uint8_t)(options));
	bt816_spi_transmit((uint8_t)(options >> 8U));
	bt816_spi_transmit((uint8_t)(hours));
	bt816_spi_transmit((uint8_t)(hours >> 8U));
	bt816_spi_transmit((uint8_t)(mins));
	bt816_spi_transmit((uint8_t)(mins >> 8U));
	bt816_spi_transmit((uint8_t)(secs));
	bt816_spi_transmit((uint8_t)(secs >> 8U));
	bt816_spi_transmit((uint8_t)(msecs));
	bt816_spi_transmit((uint8_t)(msecs >> 8U));
	bt816_cs_clear();
}

void bt816_cmd_dial(int16_t xc0, int16_t yc0, int16_t rad, uint16_t options, uint16_t val) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_DIAL);
	bt816_spi_transmit((uint8_t)((uint16_t) xc0));
	bt816_spi_transmit((uint8_t)(((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t)((uint16_t) yc0));
	bt816_spi_transmit((uint8_t)(((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t)((uint16_t) rad));
	bt816_spi_transmit((uint8_t)(((uint16_t) rad) >> 8U));
	bt816_spi_transmit((uint8_t)(options));
	bt816_spi_transmit((uint8_t)(options >> 8U));
	bt816_spi_transmit((uint8_t)(val));
	bt816_spi_transmit((uint8_t)(val >> 8U));
	bt816_spi_transmit(0U);
	bt816_spi_transmit(0U);
	bt816_cs_clear();
}

void bt816_cmd_fgcolor(uint32_t color) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FGCOLOR);
	bt816_spi_transmit((uint8_t)(color));
	bt816_spi_transmit((uint8_t)(color >> 8U));
	bt816_spi_transmit((uint8_t)(color >> 16U));
	bt816_spi_transmit(0U);
	bt816_cs_clear();
}

void bt816_cmd_gauge(int16_t xc0, int16_t yc0, int16_t rad, uint16_t options,
                    uint16_t major, uint16_t minor,
                    uint16_t val, uint16_t range)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_GAUGE);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) rad));
	bt816_spi_transmit((uint8_t) (((uint16_t) rad) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	bt816_spi_transmit((uint8_t) (major));
	bt816_spi_transmit((uint8_t) (major >> 8U));
	bt816_spi_transmit((uint8_t) (minor));
	bt816_spi_transmit((uint8_t) (minor >> 8U));
	bt816_spi_transmit((uint8_t) (val));
	bt816_spi_transmit((uint8_t) (val >> 8U));
	bt816_spi_transmit((uint8_t) (range));
	bt816_spi_transmit((uint8_t) (range >> 8U));
	bt816_cs_clear();
}

void bt816_cmd_getmatrix(int32_t *p_a, int32_t *p_b, int32_t *p_c,
                        int32_t *p_d, int32_t *p_e, int32_t *p_f)
{
	uint16_t cmdoffset;
	uint32_t address;
	bt816_cs_set();
	bt816_begin_cmd(CMD_GETMATRIX);
	bt816_spi_transmit_32(0UL);
	bt816_spi_transmit_32(0UL);
	bt816_spi_transmit_32(0UL);
	bt816_spi_transmit_32(0UL);
	bt816_spi_transmit_32(0UL);
	bt816_spi_transmit_32(0UL);
	bt816_cs_clear();

	cmdoffset = bt816_mem_read16(REG_CMD_WRITE);

	if (p_f != NULL)
	{
		address = RAM_CMD + ((cmdoffset - 4U) & 0xfffU);
		*p_f = (int32_t) bt816_mem_read32(address);
	}
	if (p_e != NULL)
	{
		address = RAM_CMD + ((cmdoffset - 8U) & 0xfffU);
		*p_e = (int32_t) bt816_mem_read32(address);
	}
	if (p_d != NULL)
	{
		address = RAM_CMD + ((cmdoffset - 12U) & 0xfffU);
		*p_d = (int32_t) bt816_mem_read32(address);
	}
	if (p_c != NULL)
	{
		address = RAM_CMD + ((cmdoffset - 16U) & 0xfffU);
		*p_c = (int32_t) bt816_mem_read32(address);
	}
	if (p_b != NULL)
	{
		address = RAM_CMD + ((cmdoffset - 20U) & 0xfffU);
		*p_b = (int32_t) bt816_mem_read32(address);
	}
	if (p_a != NULL)
	{
		address = RAM_CMD + ((cmdoffset - 24U) & 0xfffU);
		*p_a = (int32_t) bt816_mem_read32(address);
	}
}

void bt816_cmd_gradcolor(uint32_t color) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_GRADCOLOR);
	bt816_spi_transmit((uint8_t)(color));
	bt816_spi_transmit((uint8_t)(color >> 8U));
	bt816_spi_transmit((uint8_t)(color >> 16U));
	bt816_spi_transmit(0U);
	bt816_cs_clear();
}

void bt816_cmd_gradient(int16_t xc0, int16_t yc0, uint32_t rgb0, int16_t xc1,
                        int16_t yc1, uint32_t rgb1)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_GRADIENT);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) (rgb0));
	bt816_spi_transmit((uint8_t) (rgb0 >> 8U));
	bt816_spi_transmit((uint8_t) (rgb0 >> 16U));
	bt816_spi_transmit(0U);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc1));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc1) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc1));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc1) >> 8U));
	bt816_spi_transmit((uint8_t) (rgb1));
	bt816_spi_transmit((uint8_t) (rgb1 >> 8U));
	bt816_spi_transmit((uint8_t) (rgb1 >> 16U));
	bt816_spi_transmit(0U);
	bt816_cs_clear();
}

void bt816_cmd_keys(int16_t xc0, int16_t yc0, int16_t wid, int16_t hgt,
                    int16_t font, uint16_t options, const char *p_text)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_KEYS);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) wid));
	bt816_spi_transmit((uint8_t) (((uint16_t) wid) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) hgt));
	bt816_spi_transmit((uint8_t) (((uint16_t) hgt) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	private_string_write(p_text);
	bt816_cs_clear();
}

void bt816_cmd_number(int16_t xc0, int16_t yc0, int16_t font, uint16_t options,
                    int32_t number)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_NUMBER);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	bt816_spi_transmit_32((uint32_t) number);
	bt816_cs_clear();
}

void bt816_cmd_progress(int16_t xc0, int16_t yc0, int16_t wid, int16_t hgt,
                        uint16_t options, uint16_t val, uint16_t range)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_PROGRESS);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) wid));
	bt816_spi_transmit((uint8_t) (((uint16_t) wid) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) hgt));
	bt816_spi_transmit((uint8_t) (((uint16_t) hgt) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	bt816_spi_transmit((uint8_t) (val));
	bt816_spi_transmit((uint8_t) (val >> 8U));
	bt816_spi_transmit((uint8_t) (range));
	bt816_spi_transmit((uint8_t) (range >> 8U));
	bt816_spi_transmit(0U); /* dummy byte for 4-byte alignment */
	bt816_spi_transmit(0U); /* dummy byte for 4-byte alignment */
	bt816_cs_clear();
}

void bt816_cmd_romfont(uint32_t font, uint32_t romslot) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ROMFONT);
	bt816_spi_transmit_32(font);
	bt816_spi_transmit_32(romslot);
	bt816_cs_clear();
}

void bt816_cmd_rotate(int32_t angle) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_ROTATE);
	bt816_spi_transmit_32((uint32_t) angle);
	bt816_cs_clear();
}

void bt816_cmd_scale(int32_t scx, int32_t scy) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SCALE);
	bt816_spi_transmit_32((uint32_t) scx);
	bt816_spi_transmit_32((uint32_t) scy);
	bt816_cs_clear();
}

void bt816_cmd_scrollbar(int16_t xc0, int16_t yc0, int16_t width, int16_t height,
            uint16_t options, uint16_t val, uint16_t size, uint16_t range)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_SCROLLBAR);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) height));
	bt816_spi_transmit((uint8_t) (((uint16_t) height) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	bt816_spi_transmit((uint8_t) (val));
	bt816_spi_transmit((uint8_t) (val >> 8U));
	bt816_spi_transmit((uint8_t) (size));
	bt816_spi_transmit((uint8_t) (size >> 8U));
	bt816_spi_transmit((uint8_t) (range));
	bt816_spi_transmit((uint8_t) (range >> 8U));
	bt816_cs_clear();
}

void bt816_cmd_setbase(uint32_t base) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SETBASE);
	bt816_spi_transmit_32(base);
	bt816_cs_clear();
}

void bt816_cmd_setbitmap(uint32_t addr, uint16_t fmt, uint16_t width,
                        uint16_t height)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_SETBITMAP);
	bt816_spi_transmit_32(addr);
	bt816_spi_transmit((uint8_t)(fmt));
	bt816_spi_transmit((uint8_t)(fmt >> 8U));
	bt816_spi_transmit((uint8_t)(width));
	bt816_spi_transmit((uint8_t)(width >> 8U));
	bt816_spi_transmit((uint8_t)(height));
	bt816_spi_transmit((uint8_t)(height >> 8U));
	bt816_spi_transmit(0U);
	bt816_spi_transmit(0U);
	bt816_cs_clear();
}

void bt816_cmd_setfont(uint32_t font, uint32_t ptr) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SETFONT);
	bt816_spi_transmit_32(font);
	bt816_spi_transmit_32(ptr);
	bt816_cs_clear();
}

void bt816_cmd_setfont2(uint32_t font, uint32_t ptr, uint32_t firstchar) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SETFONT2);
	bt816_spi_transmit_32(font);
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit_32(firstchar);
	bt816_cs_clear();
}

void bt816_cmd_setscratch(uint32_t handle) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SETSCRATCH);
	bt816_spi_transmit_32(handle);
	bt816_cs_clear();
}

void bt816_cmd_sketch(int16_t xc0, int16_t yc0, uint16_t width, uint16_t height,
                    uint32_t ptr, uint16_t format)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_SKETCH);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) height));
	bt816_spi_transmit((uint8_t) (((uint16_t) height) >> 8U));
	bt816_spi_transmit_32(ptr);
	bt816_spi_transmit((uint8_t) (format));
	bt816_spi_transmit((uint8_t) (format >> 8U));
	bt816_spi_transmit(0U);
	bt816_spi_transmit(0U);
	bt816_cs_clear();
}

void bt816_cmd_slider(int16_t xc0, int16_t yc0, int16_t wid, int16_t hgt,
                    uint16_t options, uint16_t val, uint16_t range)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_SLIDER);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) wid));
	bt816_spi_transmit((uint8_t) (((uint16_t) wid) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) hgt));
	bt816_spi_transmit((uint8_t) (((uint16_t) hgt) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	bt816_spi_transmit((uint8_t) (val));
	bt816_spi_transmit((uint8_t) (val >> 8U));
	bt816_spi_transmit((uint8_t) (range));
	bt816_spi_transmit((uint8_t) (range >> 8U));
	bt816_spi_transmit(0U); /* dummy byte for 4-byte alignment */
	bt816_spi_transmit(0U); /* dummy byte for 4-byte alignment */
	bt816_cs_clear();
}

void bt816_cmd_spinner(int16_t xc0, int16_t yc0, uint16_t style, uint16_t scale) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_SPINNER);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) (style));
	bt816_spi_transmit((uint8_t) (style >> 8U));
	bt816_spi_transmit((uint8_t) (scale));
	bt816_spi_transmit((uint8_t) (scale >> 8U));
	bt816_cs_clear();
}

void bt816_cmd_text(int16_t xc0, int16_t yc0, int16_t font, uint16_t options,
                    const char *p_text)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_TEXT);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	private_string_write(p_text);
	bt816_cs_clear();
}

void bt816_cmd_toggle(int16_t xc0, int16_t yc0, int16_t width, int16_t font,
                    uint16_t options, uint16_t state, const char *p_text)
{
	bt816_cs_set();
	bt816_begin_cmd(CMD_TOGGLE);
	bt816_spi_transmit((uint8_t) ((uint16_t) xc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) xc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) yc0));
	bt816_spi_transmit((uint8_t) (((uint16_t) yc0) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) width));
	bt816_spi_transmit((uint8_t) (((uint16_t) width) >> 8U));
	bt816_spi_transmit((uint8_t) ((uint16_t) font));
	bt816_spi_transmit((uint8_t) (((uint16_t) font) >> 8U));
	bt816_spi_transmit((uint8_t) (options));
	bt816_spi_transmit((uint8_t) (options >> 8U));
	bt816_spi_transmit((uint8_t) (state));
	bt816_spi_transmit((uint8_t) (state >> 8U));
	private_string_write(p_text);
	bt816_cs_clear();
}

void bt816_cmd_translate(int32_t tr_x, int32_t tr_y) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_TRANSLATE);
	bt816_spi_transmit_32((uint32_t) tr_x);
	bt816_spi_transmit_32((uint32_t) tr_y);
	bt816_cs_clear();
}


uint8_t bt816_init() {
	uint32_t v = 0;
	uint8_t chip_id = 0;
	uint16_t cpu_reset = 0;
	uint16_t timeout = 0;
	delay_ms(50); /* minimum time for power-down is 5ms */
	bt816_power_down_off();
	delay_ms(100);
	delay_ms(21);
	bt816_cmd_send(0x44,0x00); // clk ext
	bt816_cmd_send(0x61,0x46); // set clock to 72 MHz
	bt816_cmd_send(0x00,0x00); // start
	delay_ms(300);
	do{
		delay_ms(1);
		chip_id = bt816_mem_read8(REG_ID);
		timeout++;
		if(timeout > 400)
		{
			return 0;
		}
	}while(chip_id != 0x7C);
	timeout = 0;
	do{
		delay_ms(1);
		cpu_reset = bt816_mem_read16(REG_CPURESET);
		timeout++;
		if(timeout > 500)
		{
			return 0;
		}
	}while(cpu_reset != 0x00);

	bt816_mem_write8(REG_PWM_DUTY, 0);

	uint32_t dev_id = bt816_mem_read32(0x0c0000);
	delay_ms(200);
	if(dev_id==BT816_ID) {
		bt816_mem_write32(REG_CMDB_WRITE,CMD_FLASHATTACH);
		delay_ms(200);
		uint8_t flash_status = bt816_mem_read8(0x3025F0);
		if(flash_status==2)	{

			bt816_cmd_flashread(0, 0, 4096);
			delay_ms(100);
			uint8_t blob_check = 1;
			for(uint16_t i=0;i<4096;i++) {
				uint8_t val = bt816_mem_read8(i);
				if(val!=unified_blob[i]) {
					blob_check = 0;
					break;
				}
			}
			if(blob_check==0) {
				bt816_cmd_memwrite(0,4096,(uint8_t*)unified_blob);
				delay_ms(100);
				bt816_cmd_flashupdate(0,0,4096);
				delay_ms(1000);
			}else {

			}

			uint32_t fast_state = bt816_cmd_flashfast();
			delay_ms(100);
			flash_status = bt816_mem_read8(0x3025F0);
			if(flash_status==3) {

				bt816_mem_write32(REG_HCYCLE, 862);
				bt816_mem_write32(REG_HOFFSET, 60);
				bt816_mem_write32(REG_HSYNC0, 16);
				bt816_mem_write32(REG_HSYNC1, 36);
				bt816_mem_write32(REG_VCYCLE, 510);
				bt816_mem_write32(REG_VOFFSET, 25);
				bt816_mem_write32(REG_VSYNC0, 7);
				bt816_mem_write32(REG_VSYNC1, 8);
				bt816_mem_write32(REG_SWIZZLE, 0);
				bt816_mem_write32(REG_PCLK_POL, 1);
				bt816_mem_write32(REG_CSPREAD, 0);
				bt816_mem_write32(REG_HSIZE, 800);
				bt816_mem_write32(REG_VSIZE, 480);

				uint16_t offset = 0;
				bt816_mem_write32(RAM_DL+offset, bt816_clear_color_rgb(0,0,0));offset+=4;
				bt816_mem_write32(RAM_DL+offset, bt816_clear(1,1,1));offset+=4;
				bt816_mem_write32(RAM_DL+offset, bt816_display());offset+=4;
				bt816_mem_write32(REG_DLSWAP, DLSWAP_FRAME);

				//demo_display1();

				//uint32_t frequency = EVE_cmd_pclkfreq(EVE_PCLK_FREQ, 0);

				bt816_mem_write16(REG_GPIOX_DIR, 0xffff);
				bt816_mem_write16(REG_GPIOX, 0xffff);

				bt816_mem_write8(REG_GPIO, 0x80);
				bt816_mem_write8(REG_PCLK, 2);

	//			v = bt816_mem_read32(REG_FRAMES);
	//			delay_ms(100);
	//			v = bt816_mem_read32(REG_FRAMES);
	//			if(v==0) return 0;
	//			delay_ms(100);
	//			v = bt816_mem_read32(REG_FRAMES);
	//			if(v==0) return 0;

				return 1;
			}
		}
	}
	return 0;
}

void bt816_cmd_fontcache(uint32_t font, uint32_t ram_addr, uint32_t cnt) {
	bt816_cs_set();
	bt816_begin_cmd(CMD_FONTCACHE);
	bt816_spi_transmit_32(font);
	bt816_spi_transmit_32(ram_addr);
	bt816_spi_transmit_32(cnt);
	bt816_cs_clear();
}

