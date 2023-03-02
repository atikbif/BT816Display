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
#define CLUSTER_REGS_CNT	16
#define NET_BITS_CNT	16
#define NET_REGS_CNT	16

#define MAX_CALC_CNT	256

void init_plc_data();
void read_calculation_config(const uint8_t *ptr);
void plc_data_calculate();
void imitate_plc_data();

#endif /* INCLUDE_PLC_DATA_H_ */
