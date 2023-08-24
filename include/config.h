/*
 * config.h
 *
 *  Created on: 2023 May 12
 *      Author: pelev
 */

#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_

#include <stdint.h>

typedef struct{
	uint8_t name[41];
	uint32_t min;
	uint32_t max;
	uint32_t value;
	uint16_t link_type;
	uint16_t link_index;
	uint16_t var_cnt;
}manage_var;

void read_config();
uint8_t get_cluster_reg_name(uint16_t num, uint8_t *buf); // return length
uint8_t get_cluster_bit_name(uint16_t num, uint8_t *buf);
uint8_t get_net_reg_name(uint16_t num, uint8_t *buf);
uint8_t get_net_bit_name(uint16_t num, uint8_t *buf);
uint8_t get_ai_meas_unit(uint8_t dev_num, uint8_t inp_num, uint8_t *buf);
uint8_t get_inp_name(uint8_t dev_num, uint8_t inp_num, uint8_t *buf);
void read_calculation_config(const uint8_t *ptr);
void read_message_conf();
uint8_t get_message_name(uint16_t num, uint8_t *buf);
uint8_t get_inp_description(uint8_t dev_num, uint8_t inp_num, uint8_t *name_buf, uint8_t *descr_buf);
uint16_t get_inp_cnt_with_descr(uint8_t dev_num);
uint16_t get_out_cnt_with_descr(uint8_t dev_num);
uint8_t get_out_description(uint8_t dev_num, uint8_t out_num, uint8_t *buf);
uint8_t get_glob_integer_name(uint16_t num, uint8_t *buf);
uint8_t get_glob_bits_name(uint16_t num, uint8_t *buf);
void read_password();
uint8_t get_manage_var(uint16_t i, manage_var *var);

uint8_t check_config_header(uint8_t *ptr);
uint32_t get_config_param_by_id(uint16_t id,uint8_t *header);
uint32_t get_config_offset_by_id(uint16_t id, uint8_t *header);
uint8_t check_item_config(uint8_t *ptr, uint16_t id);
uint16_t get_item_conf_version(uint8_t *ptr);

void read_ip_addr(uint8_t *ptr);
void read_ip_mask(uint8_t *ptr);
void read_ip_gate(uint8_t *ptr);
void set_ip_addr(uint8_t *ptr);
void set_ip_mask(uint8_t *ptr);
void set_ip_gate(uint8_t *ptr);


#endif /* INCLUDE_CONFIG_H_ */
