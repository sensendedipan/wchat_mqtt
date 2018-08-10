

#include "led_driver.h"






/**
  * @brief  led init
  * @retval None
  */
void ledInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LED_RCC_AHB1Periph, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin	= LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_REG, &GPIO_InitStructure);			
  
	GPIO_SetBits(LED1_GPIO_REG, LED1_GPIO_PIN);

    ledFlashInit();    
	printf("huayun: leds initialization OK ! \n");
}



void ledOn(uint8_t chan) 
{
	switch(chan)
	{
	case LED_STATE:
		GPIO_SetBits(LED1_GPIO_REG, LED1_GPIO_PIN);
   		
		break;
	}   
}



void ledOff(uint8_t chan) 
{
	switch(chan)
	{
	case LED_STATE:
		GPIO_ResetBits(LED1_GPIO_REG, LED1_GPIO_PIN);    		
		break;		
	}    
}



static uint8_t  sChFlashFsmSta[LED_CHANNEL]   = {2};             
static uint8_t  sChFlashCnt[LED_CHANNEL]      = {0};
static uint16_t sHwFlashOnTime[LED_CHANNEL]   = {0};
static uint16_t sHwFlashOffTime[LED_CHANNEL]  = {0};




void ledFlashInit(void)
{
	ledFlashSet(LED_STATE, 5, 250, 250);    
}



void ledFlashSet(uint8_t chan, uint8_t chFlashCnt, uint16_t hwOnTime, uint16_t hwOffTime)
{   
    uint16_t hwOn  = (hwOnTime >= 10) ? (hwOnTime/10) : hwOnTime;  //! timer is 10ms step and turn to 1ms step here
    uint16_t hwOff = (hwOffTime >= 10) ? (hwOffTime/10) : hwOffTime;
    
	switch (chan)
	{
	case LED_STATE:
		sChFlashCnt[LED_STATE]     = chFlashCnt;
		sHwFlashOnTime[LED_STATE]  = hwOn;
		sHwFlashOffTime[LED_STATE] = hwOff;
		sChFlashFsmSta[LED_STATE]  = 0;		
		break;		
	}
}



void ledFlashLoop(uint8_t chan)
{
    static uint16_t sHwTime[LED_CHANNEL]     = {0};    
    static uint8_t sChLedSta[LED_CHANNEL]    = {0};              
    static uint8_t sChFlashMode[LED_CHANNEL] = {0};             

    switch (sChFlashFsmSta[chan])
    {
	case 0:
		if (0 == sChFlashCnt[chan]) {
			sChFlashMode[chan] = 1;
			
		} else {
			sChFlashMode[chan] = 0;
		}

		ledOff(chan);                         
		sChLedSta[chan] = 1;                             
		sChFlashFsmSta[chan] = 1;                       

		break;

	case 1:
		if (sChLedSta[chan]) {
			if (0 == sHwTime[chan]) {
				if ((0 == sChFlashCnt[chan]) && (0 == sChFlashMode[chan])) {
					sChFlashFsmSta[chan] = 2;
					break;
				}

				if (sHwFlashOnTime[chan]) {
					ledOn(chan);
					sHwTime[chan] = sHwFlashOnTime[chan];     
					sChLedSta[chan] = 0;                 
				}
				
			} else {
				sHwTime[chan]--;                         
			}
			
		} else {
			if (0 == sHwTime[chan]) {
				if (sHwFlashOffTime[chan]) {
					ledOff(chan);
					sHwTime[chan] = sHwFlashOffTime[chan];
					sChLedSta[chan] = 1;

					if (!sChFlashMode[chan]) {
						if (sChFlashCnt[chan]) {
							sChFlashCnt[chan]--;
						
						} else {
							//! do nothing 
						}
					}
				}
				
			} else {
				sHwTime[chan]--;
			}
		}

		break;

	case 2:
		//! do nothing 
		break;

	default:
		break;
    }
}



void ledFlashLoopAll(void)
{
	ledFlashLoop(LED_STATE);
}



