


#include "timer5_driver.h"




/**
  * @brief  init timer5  1s
  * @note   
  */
void timer5Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER5_RCC_APB1Periph, ENABLE);
    TIM_DeInit(TIMER5_TIMER);
	TIM_TimeBaseStructure.TIM_Period = 49999;		 							

	TIM_TimeBaseStructure.TIM_Prescaler = (1680-1);	//! 10KHz			           
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIMER5_TIMER, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIMER5_TIMER, TIM_FLAG_Update);							    		
	TIM_ITConfig(TIMER5_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMER5_TIMER, ENABLE);																		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	NVIC_InitStructure.NVIC_IRQChannel = TIMER5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	
	printf("huayun: timer5 initialization at 1s OK ! \n");
}



/**
  * @brief  timer5 IRQ handler
  * @note   
  */


void TIMER5_TIMER_IRQHandler()
{
	if (TIM_GetITStatus(TIMER5_TIMER, TIM_IT_Update) != RESET) {   
		TIM_ClearITPendingBit(TIMER5_TIMER, TIM_IT_Update);
		
	}
}


