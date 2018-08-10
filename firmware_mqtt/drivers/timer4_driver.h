

#ifndef _TIMER4_DRIVER_H
#define _TIMER4_DRIVER_H


#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>


//! timer4
#define TIMER4_RCC_APB1Periph			RCC_APB1Periph_TIM4
#define TIMER4_TIMER					TIM4
#define TIMER4_IRQn              		TIM4_IRQn
#define TIMER4_TIMER_IRQHandler       	TIM4_IRQHandler


void timer4Init(void);


#endif



























