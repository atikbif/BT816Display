/*
 * cluster_state.h
 *
 *  Created on: 2023 May 20
 *      Author: Roman
 */

#ifndef USER_CLUSTER_STATE_H_
#define USER_CLUSTER_STATE_H_

#include <stdint.h>

#define IO_NAME_LENGTH		20
#define PC21_INP_CNT		14
#define PC21_OUT_CNT		6
#define DISPLAY_INP_CNT		4
#define CLUST_BIT_CNT		224
#define CLUSTER_REGS_CNT	64
#define NET_BITS_CNT		16
#define NET_REGS_CNT		16

typedef struct {
	uint8_t name[IO_NAME_LENGTH];
	uint8_t state;
	uint8_t fault;
	uint8_t used;
}dev_dinp;

typedef struct {
	uint8_t name[IO_NAME_LENGTH];
	uint8_t value;
	uint8_t tdu;
	uint16_t fault;
	uint16_t raw;
	uint8_t used;
}dev_ainp;

typedef struct {
	uint8_t name[IO_NAME_LENGTH];
	uint8_t state;
	uint8_t fault;
}dev_dout;

typedef struct {
	dev_dinp din[PC21_INP_CNT];
	dev_ainp ain[PC21_INP_CNT];
	dev_dout dout[PC21_OUT_CNT];
	uint16_t heartbeat;
	uint16_t app_id;
	uint16_t ethip_state;
}pc21_dev;

typedef struct {
	dev_dinp din[DISPLAY_INP_CNT];
	dev_ainp ain[DISPLAY_INP_CNT];
	uint16_t heartbeat;
}displ_dev;

typedef struct {
	pc21_dev pc21;
	displ_dev displ;
	uint8_t cluster_bits[CLUST_BIT_CNT];
	uint8_t net_bits[NET_BITS_CNT];
	uint16_t cluster_regs[CLUSTER_REGS_CNT];
	uint16_t net_regs[NET_REGS_CNT];
	uint32_t cur_time;
}cluster;

void init_cluster(cluster *clust);

#endif /* USER_CLUSTER_STATE_H_ */
