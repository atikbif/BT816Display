/*
 * can_task.c
 *
 *  Created on: 2023 Aug 18
 *      Author: pelev
 */

#include "can_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "can.h"
#include "can_tx_stack.h"
#include "at32f435_437_board.h"
#include "cluster_state.h"
#include "can_cmd.h"

#define DI_CNT		8

extern tx_stack can_tx_stack;
uint16_t cur_di = 0;
uint16_t prev_di = 0;
uint16_t update_di = 0;

static void update_di_data() {
	static uint16_t di_tmr=0;
	uint8_t di_mask = 0x00;
	uint8_t send_fault = 0x00;
	uint8_t send_state = 0x00;
	uint16_t di_fitted = 0x00FF;

	uint16_t cur_di = get_di_state();
	for(int i=0;i<DI_CNT;i++) {
		if((prev_di&(1<<i))!=(cur_di&(1<<i))) {
			update_di |= 1<<i;
		}
	}

	di_tmr++;
	if(di_tmr==20) {
		di_mask = update_di & di_fitted & 0xFF;
		if(di_mask)  {
			for(int i=0;i<8;i++) {
				if(di_mask & (1<<i)) {
					if(cur_di & (1<<i)) send_state |= 1<<i;
				}
			}
			send_packed_physical_digits(1,send_state,send_fault,di_mask);
			update_di &= 0xFF00;
			send_state = 0;
			send_fault = 0;
			di_tmr=0;
		}
	}
	prev_di = cur_di;
}

void can_task_function(void *pvParameters) {
	uint16_t heartbeat_tmr = 0;
	init_can_tx_stack(&can_tx_stack);
	while(1)
	{
		heartbeat_tmr++;
		if(heartbeat_tmr>=900) {
			heartbeat_tmr = 0;
			send_heartbeat();
		}
		update_di_data();
		can_write_from_stack();
		vTaskDelay(1);
	}
}
