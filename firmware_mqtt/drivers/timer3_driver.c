


#include "timer3_driver.h"




/**
  * @brief  init timer3 at 1 Hz
  * @note   
  */
void timer3Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER3_RCC_APB1Periph, ENABLE);
    TIM_DeInit(TIMER3_TIMER);
	TIM_TimeBaseStructure.TIM_Period = 49999;  
	TIM_TimeBaseStructure.TIM_Prescaler = (1680-1);  // 10KHz  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 

	TIM_TimeBaseInit(TIMER3_TIMER, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIMER3_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMER3_TIMER, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	NVIC_InitStructure.NVIC_IRQChannel = TIMER3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
	
	printf("huayun: timer3 initialization at 1s OK ! \n");
}




/**
  * @brief  timer3 IRQ handler
  * @note   
  */
//void TIMER3_TIMER_IRQHandler()
//{
//	if (TIM_GetITStatus(TIMER3_TIMER, TIM_IT_Update) != RESET) {   
//		TIM_ClearITPendingBit(TIMER3_TIMER, TIM_IT_Update);		
//	}
//}



