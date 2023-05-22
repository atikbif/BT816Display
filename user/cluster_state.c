/*
 * cluster_state.c
 *
 *  Created on: 2023 May 20
 *      Author: Roman
 */

#include "cluster_state.h"

cluster cl;

void init_cluster(cluster *clust) {
	clust->cur_time = 0;
	clust->pc21.ethip_state = 0;
	clust->pc21.app_id = 0;
	for(int i=0;i<CLUST_BIT_CNT;i++) {
		clust->cluster_bits[i] = 0;
	}
	for(int i=0;i<NET_BITS_CNT;i++) {
		clust->net_bits[i] = 0;
	}
	for(int i=0;i<CLUSTER_REGS_CNT;i++) {
		clust->cluster_regs[i] = 0;
	}
	for(int i=0;i<NET_REGS_CNT;i++) {
		clust->net_regs[i] = 0;
	}
	for(int i=0;i<PC21_INP_CNT;i++) {
		clust->pc21.ain[i].used = 0;
		clust->pc21.ain[i].fault = 0;
		clust->pc21.ain[i].raw = 0;
		clust->pc21.ain[i].tdu = 0;
		clust->pc21.ain[i].value = 0;
		clust->pc21.din[i].used = 0;
		clust->pc21.din[i].fault = 0;
		clust->pc21.din[i].state = 0;
		for(int j=0;j<IO_NAME_LENGTH;j++) {
			clust->pc21.ain[i].name[j] = 0;
			clust->pc21.din[i].name[j] = 0;
		}
	}
	for(int i=0;i<PC21_OUT_CNT;i++) {
		for(int j=0;j<IO_NAME_LENGTH;j++) {
			clust->pc21.dout[i].name[j] = 0;
		}
	}
	for(int i=0;i<PC21_INP_CNT;i++) {
		for(int j=0;j<IO_NAME_LENGTH;j++) {
			clust->pc21.ain[i].name[j] = 0;
			clust->pc21.din[i].name[j] = 0;
		}
	}
}
