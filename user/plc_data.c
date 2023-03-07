/*
 * plc_data.c
 *
 *  Created on: 2023 Feb 27
 *      Author: pelev
 */

#include "plc_data.h"

enum DI_STATE plc_di_state[DI_CNT];
enum DO_STATE plc_do_state[DO_CNT];
enum DI_TYPE plc_di_type[DI_CNT];
enum AI_TYPE plc_ai_type[AI_CNT];

uint16_t plc_ain_raw[AI_CNT];
uint8_t plc_clust_bits[CLUST_BITS_CNT];
uint16_t plc_clust_regs[CLUSTER_REGS_CNT];
uint8_t plc_net_bits[NET_BITS_CNT];
uint16_t plc_net_regs[NET_REGS_CNT];

uint16_t calc_total_cnt = 0;

calc_config calc[MAX_CALC_CNT];

void init_plc_data() {
	for(uint16_t i=0;i<DI_CNT;i++) {
		plc_di_state[i] = DI_OFF;
		plc_di_type[i] = DI_UNUSED;
	}
	for(uint16_t i=0;i<DO_CNT;i++) {
		plc_do_state[i] = DO_OFF;
	}
	for(uint16_t i=0;i<CLUST_BITS_CNT;i++) {
		plc_clust_bits[i] = 0;
	}
	for(uint16_t i=0;i<CLUSTER_REGS_CNT;i++) {
		plc_clust_regs[i] = 0;
	}
	for(uint16_t i=0;i<NET_BITS_CNT;i++) {
		plc_net_bits[i] = 0;
	}
	for(uint16_t i=0;i<NET_REGS_CNT;i++) {
		plc_net_regs[i] = 0;
	}
}

void read_calculation_config(const uint8_t *ptr) {
	// imitation read from flash
	calc_total_cnt = 4;
	for(uint16_t i=0;i<calc_total_cnt;i++) {
		calc[i].k = 2.5;
		calc[i].b = 10;
		calc[i].link = LINK_RAW;
		calc[i].index = i;
		calc[i].result = 0;
		calc[i].prec = PR2;
	}
}

void plc_data_calculate() {
	for(uint16_t i=0;i<calc_total_cnt;i++) {
		int32_t source_data = 0;
		switch(calc[i].link) {
			case LINK_RAW:
				if(calc[i].index<AI_CNT) source_data = plc_ain_raw[calc[i].index];
				break;
			case LINK_2RAW:
				if(calc[i].index<AI_CNT-1) {
					source_data = plc_ain_raw[calc[i].index];
					source_data = source_data << 16;
					source_data |= plc_ain_raw[calc[i].index+1];
				}
				break;
			case LINK_CLUST_REGS:
				if(calc[i].index<CLUSTER_REGS_CNT) source_data = plc_clust_regs[calc[i].index];
				break;
			case LINK_2CLUST_REGS:
				if(calc[i].index<CLUSTER_REGS_CNT-1) {
					source_data = plc_clust_regs[calc[i].index];
					source_data = source_data << 16;
					source_data |= plc_clust_regs[calc[i].index+1];
				}
				break;
			case LINK_NET_REGS:
				if(calc[i].index<NET_REGS_CNT) source_data = plc_net_regs[calc[i].index];
				break;
			case LINK_2NET_REGS:
				if(calc[i].index<NET_REGS_CNT-1) {
					source_data = plc_net_regs[calc[i].index];
					source_data = source_data << 16;
					source_data |= plc_net_regs[calc[i].index+1];
				}
				break;
		}
		float corrected_data = source_data*calc[i].k + calc[i].b;
		switch(calc[i].prec) {
			case PR0:
				if(corrected_data<0) {
					calc[i].result = (int32_t)(corrected_data - 0.5);
				}else {
					calc[i].result = (int32_t)(corrected_data + 0.5);
				}
				break;
			case PR1:
				if(corrected_data<0) {
					calc[i].result = (int32_t)(corrected_data*10 - 0.5);
				}else {
					calc[i].result = (int32_t)(corrected_data*10 + 0.5);
				}
				break;
			case PR2:
				if(corrected_data<0) {
					calc[i].result = (int32_t)(corrected_data*100 - 0.5);
				}else {
					calc[i].result = (int32_t)(corrected_data*100 + 0.5);
				}
				break;
			case PR3:
				if(corrected_data<0) {
					calc[i].result = (int32_t)(corrected_data*1000 - 0.5);
				}else {
					calc[i].result = (int32_t)(corrected_data*1000 + 0.5);
				}
				break;
		}
	}
}

void imitate_plc_data() {
	static uint8_t cnt = 0;
	const uint16_t discr[16] = {0x0300, 0x0201, 0x1001, 0x1003, 0x1000, 0x3002, 0x20FF, 0x00FF,
								0x0107, 0x0207, 0x0407, 0x0800, 0x1000, 0x2001, 0x3001, 0x0001};
	const uint16_t analogue[4][16] = {
			{10118, 10125, 10127, 10127, 10250, 11000, 11400, 12000, 11400, 11000, 10300, 10150, 10100, 10105, 10115, 10120},
			{11118, 11125, 11127, 11127, 11250, 12000, 12400, 13000, 12400, 12000, 11300, 11150, 11100, 11105, 11115, 11120},
			{21118, 21125, 21127, 21127, 21250, 22000, 22400, 23000, 22400, 22000, 21300, 21150, 21100, 21105, 21115, 21120},
			{1118, 1125, 1127, 1127, 1250, 2000, 2400, 2000, 2400, 2000, 1300, 1150, 1100, 1105, 1115, 1120}
	};

	if(discr[cnt]&0x01) plc_di_state[0] = DI_ON; else plc_di_state[0] = DI_OFF;
	if(discr[cnt]&0x02) plc_di_state[1] = DI_ON; else plc_di_state[1] = DI_OFF;
	if(discr[cnt]&0x04) plc_di_state[2] = DI_ON; else plc_di_state[2] = DI_OFF;
	if(discr[cnt]&0x08) plc_di_state[3] = DI_FAULT; else plc_di_state[3] = DI_OFF;
	if(discr[cnt]&0x10) plc_di_state[4] = DI_FAULT; else plc_di_state[4] = DI_OFF;
	if(discr[cnt]&0x20) plc_di_state[5] = DI_FAULT; else plc_di_state[5] = DI_OFF;
	if(discr[cnt]&0x40) plc_di_state[6] = DI_FAULT; else plc_di_state[6] = DI_OFF;
	if(discr[cnt]&0x80) plc_di_state[7] = DI_FAULT; else plc_di_state[7] = DI_OFF;

	if(discr[cnt]&0x0100) plc_do_state[0] = DO_ON; else plc_do_state[0] = DO_OFF;
	if(discr[cnt]&0x0200) plc_do_state[1] = DO_ON; else plc_do_state[1] = DO_OFF;
	if(discr[cnt]&0x0400) plc_do_state[2] = DO_ON; else plc_do_state[2] = DO_OFF;
	if(discr[cnt]&0x0800) plc_do_state[3] = DO_ON; else plc_do_state[3] = DO_OFF;
	if(discr[cnt]&0x1000) plc_do_state[4] = DO_FAULT; else plc_do_state[4] = DO_OFF;
	if(discr[cnt]&0x2000) plc_do_state[5] = DO_ON; else plc_do_state[5] = DO_OFF;

	plc_ain_raw[0] = analogue[0][cnt];
	plc_ain_raw[1] = analogue[1][cnt];
	plc_ain_raw[2] = analogue[2][cnt];
	plc_ain_raw[3] = analogue[3][cnt];
	plc_ain_raw[4] = analogue[0][cnt]+analogue[1][cnt];
	plc_ain_raw[5] = analogue[1][cnt]+analogue[2][cnt];
	plc_ain_raw[6] = analogue[2][cnt]+analogue[3][cnt];
	plc_ain_raw[4] = analogue[3][cnt]+analogue[4][cnt];

	plc_clust_regs[0]+=7;if(plc_clust_regs[0]>=1100) plc_clust_regs[0] = 0;
	plc_clust_regs[1]+=37;if(plc_clust_regs[1]>=1000) plc_clust_regs[1] = 0;
	plc_clust_regs[2]+=23;if(plc_clust_regs[2]>=1000) plc_clust_regs[2] = 0;
	plc_clust_regs[3]+=71;if(plc_clust_regs[3]>=1100) plc_clust_regs[3] = 0;
	plc_clust_regs[4]+=12;if(plc_clust_regs[4]>=1100) plc_clust_regs[4] = 0;

	cnt++;
	if(cnt>=16) cnt = 0;
}

