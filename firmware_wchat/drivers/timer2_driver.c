

#include "timer2_driver.h"
#include "led_driver.h"
#include "data_struct.h"

static uint8_t timer2_1000ms = 0;





/**
  * @brief  init timer2  10ms
  * @note   
  */
void timer2Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER2_RCC_APB1Periph, ENABLE);
    TIM_DeInit(TIMER2_TIMER);
	TIM_TimeBaseStructure.TIM_Period = 499;		 							

	TIM_TimeBaseStructure.TIM_Prescaler = (1680-1);	//! 10KHz			           
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIMER2_TIMER, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIMER2_TIMER, TIM_FLAG_Update);							    		
	TIM_ITConfig(TIMER2_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMER2_TIMER, ENABLE);																		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	NVIC_InitStructure.NVIC_IRQChannel = TIMER2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
}




/**
  * @brief  timer2 IRQ handler
  * @note   
  */
void TIMER2_TIMER_IRQHandler()
{
	if (TIM_GetITStatus(TIMER2_TIMER, TIM_IT_Update) != RESET) {   
		TIM_ClearITPendingBit(TIMER2_TIMER, TIM_IT_Update);

        if (timer2_1000ms++ == 99) {
            timer2_1000ms = 0;
        }
		ledFlashLoopAll();
	}

}






