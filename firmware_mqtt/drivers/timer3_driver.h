

#ifndef _TIMER3_DRIVER_H
#define _TIMER3_DRIVER_H


#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>



//! timer3
#define TIMER3_RCC_APB1Periph			RCC_APB1Periph_TIM3
#define TIMER3_TIMER					TIM3
#define TIMER3_IRQn              		TIM3_IRQn
#define TIMER3_TIMER_IRQHandler       	TIM3_IRQHandler

void timer3Init(void);

#endif




