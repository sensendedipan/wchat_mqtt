

#ifndef _LED_DRIVER_H
#define _LED_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"


#define LED_CHANNEL	2

#define LED1_GPIO_REG 		GPIOE
#define LED1_GPIO_PIN		GPIO_Pin_13		//! PE.13
    

#define LED_RCC_AHB1Periph	   (RCC_AHB1Periph_GPIOE)


typedef enum 
{
	LED_STATE,
    
} led_channel_t; 







void ledInit(void);
void ledOn(uint8_t chan);
void ledOff(uint8_t chan);

void ledFlashInit(void);
void ledFlashSet(uint8_t chan, uint8_t chFlashCnt, uint16_t hwOnTime, uint16_t hwOffTime);
void ledFlashLoop(uint8_t chan);
void ledFlashLoopAll(void);



#endif


