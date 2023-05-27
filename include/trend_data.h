/*
 * trend_data.h
 *
 *  Created on: 2023 May 27
 *      Author: pelev
 */

#ifndef INCLUDE_TREND_DATA_H_
#define INCLUDE_TREND_DATA_H_

#include <stdint.h>

#define TREND_SIZE		64

typedef struct {
	uint8_t data;
	uint32_t time;
}trend_point;

typedef struct {
	trend_point point[TREND_SIZE];
	uint16_t first_point;
	uint16_t cur_point;
	uint8_t min_warn;
	uint8_t min_alarm;
	uint8_t max_warn;
	uint8_t max_alarm;
	uint8_t inp_num;
	uint8_t dev_addr;
	uint8_t full_flag;
}trend;

trend get_new_trend();
void init_trends();
void add_data_to_trend(trend *tr, trend_point point);
uint8_t add_trend(trend tr);
uint8_t get_trend_cnt();
void delete_trend(uint8_t trend_num);
uint8_t get_trend_max_cnt();
trend* get_trend_by_num(uint8_t num);
void save_trends();

#endif /* INCLUDE_TREND_DATA_H_ */
