
#ifndef _TIMER2_DRIVER_H
#define _TIMER2_DRIVER_H


#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>



//! timer2
#define TIMER2_RCC_APB1Periph			RCC_APB1Periph_TIM2
#define TIMER2_TIMER					TIM2
#define TIMER2_IRQn              		TIM2_IRQn
#define TIMER2_TIMER_IRQHandler       	TIM2_IRQHandler

void timer2Init(void);

#endif


