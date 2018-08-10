


#ifndef _USART2_DRIVER_H
#define _USART2_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"


//! usart2/config

#define USART2_RCC_AHB1Periph			RCC_AHB1Periph_GPIOA
#define	USART2_RCC_APB1Periph_USART2	RCC_APB1Periph_USART2
#define	USART2_GPIO						GPIOA
#define	USART2_GPIO_PIN_TX				GPIO_Pin_2	//! PA.02
#define	USART2_GPIO_PIN_RX				GPIO_Pin_3  //! PA.03
#define	USART2_GPIO_PIN_SOURCE_TX		GPIO_PinSource2 
#define	USART2_GPIO_PIN_SOURCE_RX		GPIO_PinSource3 
#define USART2_USART					USART2
#define USART2_USART_IRQHandler			USART2_IRQHandler
#define	USART2_USART_IRQn				USART2_IRQn


void usart2Init(void);
void usart2Stop(void);
void usart2SendChar(const uint8_t data);
void usart2SendBuffer(const uint8_t *buffer, uint16_t count);
void usart2SendString(const uint8_t *s); 


#endif


