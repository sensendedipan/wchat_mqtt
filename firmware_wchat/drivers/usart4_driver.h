


#ifndef _USART4_DRIVER_H
#define _USART4_DRIVER_H


#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"


#define USART4_RCC_AHB1Periph			RCC_AHB1Periph_GPIOC
#define	USART4_RCC_APB1Periph_USART4	RCC_APB1Periph_UART4
#define	USART4_GPIO						GPIOC
#define	USART4_GPIO_PIN_TX				GPIO_Pin_10	//! PC.10
#define	USART4_GPIO_PIN_RX				GPIO_Pin_11	//! PC.11
#define	USART4_GPIO_PIN_SOURCE_TX		GPIO_PinSource10 //! PC.10
#define	USART4_GPIO_PIN_SOURCE_RX		GPIO_PinSource11 //! PC.11
#define USART4_USART					UART4
#define USART4_USART_IRQHandler			UART4_IRQHandler
#define	USART4_USART_IRQn				UART4_IRQn



void usart4Init(void);
void usart4Stop(void);
void usart4SendChar(const uint8_t data);
void usart4SendBuffer(const uint8_t *buffer, uint16_t count);


#endif
