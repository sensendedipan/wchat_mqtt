#ifndef _ADC_DRIVER_H
#define _ADC_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"
#include "debug.h"


void adcInit(void);
int8_t getTemperature(void);


#endif
