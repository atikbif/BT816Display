/*
 * cur_time.c
 *
 *  Created on: 2023 May 21
 *      Author: Roman
 */

#include "cur_time.h"
#include "at32f435_437_board.h"
#include "at32f435_437_clock.h"

#define JD0               2451911         // дней до 01 янв 2001 ПН

ertc_time_type dev_time;
__IO uint16_t ertc_clk_div_a = 0;
__IO uint16_t ertc_clk_div_b = 0;

void init_cur_time() {
	crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
	pwc_battery_powered_domain_access(TRUE);

	if (ertc_bpr_data_read(ERTC_DT1) != 0x1234)
	{
		crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);
		pwc_battery_powered_domain_access(TRUE);
		crm_battery_powered_domain_reset(TRUE);
		crm_battery_powered_domain_reset(FALSE);

		crm_clock_source_enable(CRM_CLOCK_SOURCE_LEXT, TRUE);

		/* wait till lext is ready */
		while(crm_flag_get(CRM_LEXT_STABLE_FLAG) == RESET)
		{
		}

		crm_ertc_clock_select(CRM_ERTC_CLOCK_LEXT);

		/* ertc second(1hz) = ertc_clk / (ertc_clk_div_a + 1) * (ertc_clk_div_b + 1) */
		ertc_clk_div_b = 255;
		ertc_clk_div_a = 127;

		crm_ertc_clock_enable(TRUE);

		ertc_reset();
		ertc_wait_update();
		ertc_divider_set(ertc_clk_div_a, ertc_clk_div_b);
		ertc_hour_mode_set(ERTC_HOUR_MODE_24);
		ertc_date_set(23, 5, 23, 2);
		ertc_time_set(0, 0, 0, ERTC_24H);
		ertc_bpr_data_write(ERTC_DT1, 0x1234);
	}
}

uint32_t time_to_uint32() {
	uint32_t   JDN;
	uint16_t   y;
	uint8_t      m, a;

	ertc_calendar_get(&dev_time);
	a = (14 - dev_time.month)/12;
	y = dev_time.year +2000 + 4800 - a;            //2000 т.к. у нас дата идет от 0 до 99
	m = dev_time.month + (12*a) - 3;
	JDN = dev_time.day;
	JDN+= (153*m + 2)/5;
	JDN+= 365*y;
	JDN+= y/4;
	JDN+= -y/100;
	JDN+= y/400;
	JDN-= 32045;
	JDN-= JD0;                              // так как счетчик у нас нерезиновый, уберем дни которые прошли до 01 янв 2001
	JDN*= 86400;                           // переводим дни в секунды
	JDN+= (dev_time.hour*3600);                  // и дополняем его секундами текущего дня
	JDN+= (dev_time.min*60);
	JDN+= (dev_time.sec);
	return JDN;
}

void time_from_uint32(uint32_t value) {
	uint32_t ace;
	uint8_t b,d,m;

	ace = (value/86400) + 32044 + JD0;
	b = (4*ace + 3)/146097;
	ace = ace - ((146097*b)/4);
	d = (4*ace + 3)/1461;
	ace = ace - ((1461*d)/4);
	m = (5*ace + 2)/153;
	uint8_t date = ace - ((153*m+2)/5) + 1;
	uint8_t month = m + 3 - (12*(m/10));
	uint8_t year = 100*b + d-4800 + (m/10) - 2000;
	uint8_t hour = (value/3600)%24;
	uint8_t min = (value/60)%60;
	uint8_t sec = (value%60);
	ertc_date_set(year, month, date, 1);
	ertc_time_set(hour, min, sec, ERTC_24H);
}

void update_time_in_cluster(cluster *clust) {
	uint32_t t = time_to_uint32();
	clust->cur_time = t;
}
