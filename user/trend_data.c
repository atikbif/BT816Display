/*
 * trend_data.c
 *
 *  Created on: 2023 May 27
 *      Author: pelev
 */

#include "trend_data.h"
#include "at32f435_437_board.h"
#include "cluster_state.h"

#define TREND_MAX_CNT	3

static trend trends[TREND_MAX_CNT];
uint8_t tr_cnt = 0;

trend get_new_trend() {
	trend tr;
	tr.dev_addr = 1;
	tr.first_point = 0;
	tr.cur_point = 0;
	tr.inp_num = 1;
	tr.max_alarm = 255;
	tr.max_warn = 255;
	tr.min_alarm = 0;
	tr.min_warn = 0;
	tr.full_flag = 0;
	for(int i=0;i<TREND_SIZE;i++) {
		tr.point[i].data = 0;
		tr.point[i].time = 0;
	}
	return tr;
}
void init_trends() {
	for(int i=0;i<TREND_MAX_CNT;i++) {
		trends[i] = get_new_trend();
	}
	if (ertc_bpr_data_read(ERTC_DT6) != 0x1234)
	{
		ertc_bpr_data_write(ERTC_DT6, 0x1234);
		ertc_bpr_data_write(ERTC_DT7, 0x0000);	// trend cnt
		ertc_bpr_data_write(ERTC_DT8, 0x0101);	// dev addr and input num
		ertc_bpr_data_write(ERTC_DT9, 0x0000);	// min warning and min alarm
		ertc_bpr_data_write(ERTC_DT10, 0xFFFF); // max warning and max alarm
		ertc_bpr_data_write(ERTC_DT11, 0x0101);	// dev addr and input num
		ertc_bpr_data_write(ERTC_DT12, 0x0000);	// min warning and min alarm
		ertc_bpr_data_write(ERTC_DT13, 0xFFFF); // max warning and max alarm
		ertc_bpr_data_write(ERTC_DT14, 0x0101);	// dev addr and input num
		ertc_bpr_data_write(ERTC_DT15, 0x0000);	// min warning and min alarm
		ertc_bpr_data_write(ERTC_DT16, 0xFFFF); // max warning and max alarm
	}
	tr_cnt = ertc_bpr_data_read(ERTC_DT7);
	if(tr_cnt>TREND_MAX_CNT) tr_cnt = TREND_MAX_CNT;
	uint16_t val = ertc_bpr_data_read(ERTC_DT8);
	trends[0].dev_addr = val>>8;
	trends[0].inp_num = val&0xFF;
	if(trends[0].inp_num==0) trends[0].inp_num = 1;
	if(trends[0].inp_num>PC21_INP_CNT) trends[0].inp_num=PC21_INP_CNT;
	val = ertc_bpr_data_read(ERTC_DT9);
	trends[0].min_warn = val>>8;
	trends[0].min_alarm = val&0xFF;
	val = ertc_bpr_data_read(ERTC_DT10);
	trends[0].max_warn = val>>8;
	trends[0].max_alarm = val&0xFF;

	val = ertc_bpr_data_read(ERTC_DT11);
	trends[1].dev_addr = val>>8;
	trends[1].inp_num = val&0xFF;
	if(trends[1].inp_num==0) trends[0].inp_num = 1;
	if(trends[1].inp_num>PC21_INP_CNT) trends[0].inp_num=PC21_INP_CNT;
	val = ertc_bpr_data_read(ERTC_DT12);
	trends[1].min_warn = val>>8;
	trends[1].min_alarm = val&0xFF;
	val = ertc_bpr_data_read(ERTC_DT13);
	trends[1].max_warn = val>>8;
	trends[1].max_alarm = val&0xFF;

	val = ertc_bpr_data_read(ERTC_DT14);
	trends[2].dev_addr = val>>8;
	trends[2].inp_num = val&0xFF;
	if(trends[2].inp_num==0) trends[0].inp_num = 1;
	if(trends[2].inp_num>PC21_INP_CNT) trends[0].inp_num=PC21_INP_CNT;
	val = ertc_bpr_data_read(ERTC_DT15);
	trends[2].min_warn = val>>8;
	trends[2].min_alarm = val&0xFF;
	val = ertc_bpr_data_read(ERTC_DT16);
	trends[2].max_warn = val>>8;
	trends[2].max_alarm = val&0xFF;
}

void add_data_to_trend(trend *tr, trend_point point) {
	tr->point[tr->cur_point] = point;
	tr->cur_point++;
	if(tr->cur_point>=TREND_SIZE) {
		tr->cur_point = 0;
		tr->full_flag = 1;
	}
	if(tr->full_flag) {
		tr->first_point++;
		if(tr->first_point>=TREND_SIZE) tr->first_point = 0;
	}
}

uint8_t add_trend(trend tr) {
	if(tr_cnt>=TREND_MAX_CNT) return 0;
	trends[tr_cnt] = tr;
	tr_cnt++;
	return 1;
}

uint8_t get_trend_cnt() {
	return tr_cnt;
}

void delete_trend(uint8_t trend_num) {
	if(trend_num && trend_num<=tr_cnt) {
		while(trend_num<TREND_MAX_CNT) {
			trends[trend_num-1] = trends[trend_num];
			trend_num++;
		}
		trends[TREND_MAX_CNT-1] = get_new_trend();
		tr_cnt--;
	}
}

uint8_t get_trend_max_cnt() {
	return TREND_MAX_CNT;
}

trend* get_trend_by_num(uint8_t num) {
	if(num && num<=TREND_MAX_CNT) {
		return &trends[num-1];
	}
	return 0;
}

void save_trends() {
	uint16_t val = tr_cnt;
	ertc_bpr_data_write(ERTC_DT7, val);	// trend cnt

	val = trends[0].dev_addr;
	val = val << 8;
	val |= trends[0].inp_num;
	ertc_bpr_data_write(ERTC_DT8, val);	// dev addr and input num
	val = trends[0].min_warn;
	val = val << 8;
	val |= trends[0].min_alarm;
	ertc_bpr_data_write(ERTC_DT9, val);	// min warning and min alarm
	val = trends[0].max_warn;
	val = val << 8;
	val |= trends[0].max_alarm;
	ertc_bpr_data_write(ERTC_DT10, val); // max warning and max alarm

	val = trends[1].dev_addr;
	val = val << 8;
	val |= trends[1].inp_num;
	ertc_bpr_data_write(ERTC_DT11, val);	// dev addr and input num
	val = trends[1].min_warn;
	val = val << 8;
	val |= trends[1].min_alarm;
	ertc_bpr_data_write(ERTC_DT12, val);	// min warning and min alarm
	val = trends[1].max_warn;
	val = val << 8;
	val |= trends[1].max_alarm;
	ertc_bpr_data_write(ERTC_DT13, val); // max warning and max alarm

	val = trends[2].dev_addr;
	val = val << 8;
	val |= trends[2].inp_num;
	ertc_bpr_data_write(ERTC_DT14, val);	// dev addr and input num
	val = trends[2].min_warn;
	val = val << 8;
	val |= trends[2].min_alarm;
	ertc_bpr_data_write(ERTC_DT15, val);	// min warning and min alarm
	val = trends[2].max_warn;
	val = val << 8;
	val |= trends[2].max_alarm;
	ertc_bpr_data_write(ERTC_DT16, val); // max warning and max alarm
}

