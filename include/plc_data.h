/*
 * plc_data.h
 *
 *  Created on: 2023 Feb 27
 *      Author: pelev
 */

#ifndef INCLUDE_PLC_DATA_H_
#define INCLUDE_PLC_DATA_H_

#include <stdint.h>

#define DI_CNT	14
#define DO_CNT	6
#define AI_CNT	14
#define CLUST_BITS_CNT	224
#define CLUSTER_REGS_CNT	64
#define NET_BITS_CNT	16
#define NET_REGS_CNT	16

#define MAX_CALC_CNT	256


enum DI_STATE {DI_ON, DI_OFF, DI_FAULT};
enum DO_STATE {DO_ON, DO_OFF, DO_FAULT};
enum AI_STATE {AI_NORMAL, AI_FAULT};
enum AI_TYPE {AI_UNUSED, AI_U, AI_I};
enum DI_TYPE {DI_UNUSED, DI_USED};

enum PRECISION_TYPE {PR0, PR1, PR2, PR3};
enum CALC_LINK {LINK_RAW, LINK_2RAW, LINK_CLUST_REGS, LINK_2CLUST_REGS, LINK_NET_REGS, LINK_2NET_REGS, CALC_LAST_LINK};

typedef struct  {
	enum PRECISION_TYPE prec;
	float k;
	float b;
	enum CALC_LINK link;
	uint16_t index;
	int32_t result;
	float fl_res;
}calc_config;

void init_plc_data();
void reset_plc_data();
void read_calculation_config(const uint8_t *ptr);
void plc_data_calculate();
void imitate_plc_data();

#endif /* INCLUDE_PLC_DATA_H_ */
