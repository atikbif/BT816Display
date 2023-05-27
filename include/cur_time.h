/*
 * cur_time.h
 *
 *  Created on: 2023 May 21
 *      Author: Roman
 */

#ifndef INCLUDE_CUR_TIME_H_
#define INCLUDE_CUR_TIME_H_

#include <stdint.h>
#include "cluster_state.h"

#define JD0               2451911         // дней до 01 янв 2001 ПН

void init_cur_time();
uint32_t time_to_uint32();
void time_from_uint32(uint32_t value);
void update_time_in_cluster(cluster *clust);

#endif /* INCLUDE_CUR_TIME_H_ */
