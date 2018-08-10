


#ifndef _RTC_DRIVER_H
#define _RTC_DRIVER_H

#include "stm32f4xx.h" 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>  
#include "nmea.h"

bool rtcInit(void);
void rtcSetDate(uint8_t year, uint8_t month, uint8_t day);
void rtcSetTime(uint8_t hour, uint8_t min, uint8_t sec);
void rtcGetTime(gps_state_t *t);

#endif

