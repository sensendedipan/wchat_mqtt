


#ifndef _SIM_DRIVER_H
#define _SIM_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"


#define	SIM_GPIO_REG_GPS_EN		GPIOC
#define SIM_GPIO_PIN_GPS_EN		GPIO_Pin_13

#define	SIM_GPIO_REG_PWR_EN	GPIOD
#define	SIM_GPIO_PIN_PWR_EN	GPIO_Pin_1
#define	SIM_GPIO_REG_CTRL	GPIOD
#define	SIM_GPIO_PIN_CTRL	GPIO_Pin_0


#define	SIM_RCC_AHB1Periph		RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD 


void simInit(void);
void simInnerPowerReset(void);
void simOuterPowerReset(void);


#endif


