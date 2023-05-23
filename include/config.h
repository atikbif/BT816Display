/*
 * config.h
 *
 *  Created on: 2023 May 12
 *      Author: pelev
 */

#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_

#include <stdint.h>

void read_config();
uint8_t get_cluster_reg_name(uint16_t num, uint8_t *buf); // return length
uint8_t get_cluster_bit_name(uint16_t num, uint8_t *buf);
uint8_t get_net_reg_name(uint16_t num, uint8_t *buf);
uint8_t get_net_bit_name(uint16_t num, uint8_t *buf);
uint8_t get_ai_meas_unit(uint8_t dev_num, uint8_t inp_num, uint8_t *buf);
uint8_t get_inp_name(uint8_t dev_num, uint8_t inp_num, uint8_t *buf);
void read_calculation_config(const uint8_t *ptr);
uint8_t get_inp_description(uint8_t dev_num, uint8_t inp_num, uint8_t *buf);
uint16_t get_inp_cnt_with_descr(uint8_t dev_num);
uint16_t get_out_cnt_with_descr(uint8_t dev_num);
uint8_t get_out_description(uint8_t dev_num, uint8_t out_num, uint8_t *buf);
uint8_t get_glob_integer_name(uint16_t num, uint8_t *buf);
uint8_t get_glob_bits_name(uint16_t num, uint8_t *buf);

#endif /* INCLUDE_CONFIG_H_ */
