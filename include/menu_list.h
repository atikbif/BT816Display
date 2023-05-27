/*
 * menu_list.h
 *
 *  Created on: 2023 May 10
 *      Author: pelev
 */

#ifndef INCLUDE_MENU_LIST_H_
#define INCLUDE_MENU_LIST_H_

enum MENU_LIST {MENU_MAIN, MENU_APPL_INFO, MENU_CLUST_INFO, MENU_ALARM_LIST, MENU_PASSWD, MENU_CLR_ALARMS, MENU_DIAGNOSTIC,
	MENU_DIAGNOSTIC2, MENU_PC21_STATE, MENU_CROSS_REF, MENU_GLOB_INT, MENU_GLOB_BITS, MENU_CONFIG, MENU_SET_TIME,
	MENU_SYS_CFG, MENU_EDIT_VAR, MENU_PROG, MENU_TREND_CONF, MENU_ADD_TREND,
	MENU_HELP, MENU_PC21, MENU_MNEMO, MENU_MANAGE, MENU_DIAGN, MENU_DI,
	MENU_DO,MENU_AI,MENU_CLUST_BITS,MENU_NET_BITS,MENU_CLUST_REGS,MENU_NET_REGS,MENU_EDIT_U16,MENU_LIMIT};

typedef enum MENU_LIST menu_list_t;

#endif /* INCLUDE_MENU_LIST_H_ */
