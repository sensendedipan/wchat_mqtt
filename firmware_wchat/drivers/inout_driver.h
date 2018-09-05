

#ifndef _INOUT_DRIVER_H
#define _INOUT_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"
#include "data_struct.h"

void iosInit(void);

//! reset key(digital input) just for parameters
#define RESET_GPIO_REG 			GPIOE
#define RESET_GPIO_PIN			GPIO_Pin_1		//! PE.01	
#define RESET_RCC_AHB1Periph	RCC_AHB1Periph_GPIOE


//! reset key
void resetkeyInit(void);
bool resetkeyPressDown(void);


#endif


