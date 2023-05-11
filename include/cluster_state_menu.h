/*
 * cluster_state_menu.h
 *
 *  Created on: 2023 May 10
 *      Author: pelev
 */

#ifndef INCLUDE_CLUSTER_STATE_MENU_H_
#define INCLUDE_CLUSTER_STATE_MENU_H_

#include <stdint.h>

typedef struct {
	uint8_t plc_type[8];
	uint8_t plc_link[8];
	uint8_t used_plc[8];
}cluster_info_data_type;

enum PLC_TYPE {PC21_1=1, PC21_CD, PC21_MC, PC21_2T};
typedef enum PLC_TYPE plc_type_t;

void cluster_state_menu(uint16_t key);

#endif /* INCLUDE_CLUSTER_STATE_MENU_H_ */
