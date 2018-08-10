



#include "board.h"
#include "global.h"
#include "gprs.h"






/**
  * @brief  init timer6 
  * @note   10ms
  */
void timer6Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER6_RCC_APB1Periph, ENABLE);
    TIM_DeInit(TIMER6_TIMER);
	TIM_TimeBaseStructure.TIM_Period = 99;  
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);  // 10KHz  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 

	TIM_TimeBaseInit(TIMER6_TIMER, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIMER6_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMER6_TIMER, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	NVIC_InitStructure.NVIC_IRQChannel = TIMER6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
	
	printf("huayun: timer6 initialization at 10 ms OK ! \n");
}




/**
  * @brief  timer6 IRQ handler
  * @note   
  */
void TIMER6_TIMER_IRQHandler()
{
	if (TIM_GetITStatus(TIMER6_TIMER, TIM_IT_Update) != RESET) {   
		TIM_ClearITPendingBit(TIMER6_TIMER, TIM_IT_Update);
		ledFlashLoopAll();	

	}
}



