/**
******************************************************************************
* @file    			timer_driver.c
* @author  			apple
* @version 			V1.0
* @date    			2017-07-31
* @brief   			timer4 init
* @attention		
******************************************************************************
*/



#include "timer4_driver.h"



/**
  * @brief  init timer4 
  * @note   100ms
  */
void timer4Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER4_RCC_APB1Periph, ENABLE);
    TIM_DeInit(TIMER4_TIMER);
	TIM_TimeBaseStructure.TIM_Period = 50000;  
	TIM_TimeBaseStructure.TIM_Prescaler = (168-1);    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 

	TIM_TimeBaseInit(TIMER4_TIMER, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIMER4_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMER4_TIMER, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	NVIC_InitStructure.NVIC_IRQChannel = TIMER4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
	
	printf("huayun: timer4 initialization at 100ms OK ! \n");
}






/**
  * @brief  timer4 IRQ handler
  * @note   for gprs at cmd 
  */
#if 0
void TIMER4_TIMER_IRQHandler()
{
	if (TIM_GetITStatus(TIMER4_TIMER, TIM_IT_Update) != RESET) {   
		TIM_ClearITPendingBit(TIMER4_TIMER, TIM_IT_Update);
					
	}
}

#endif





























