


#ifndef _USART6_DRIVER_H
#define _USART6_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>


//! usart6/user config

#define USART6_RCC_AHB1Periph			RCC_AHB1Periph_GPIOC
#define	USART6_RCC_APB2Periph_USART6	RCC_APB2Periph_USART6
#define	USART6_GPIO						GPIOC
#define	USART6_GPIO_PIN_TX				GPIO_Pin_6	//!
#define	USART6_GPIO_PIN_RX				GPIO_Pin_7 //! 
#define	USART6_GPIO_PIN_SOURCE_TX		GPIO_PinSource6 
#define	USART6_GPIO_PIN_SOURCE_RX		GPIO_PinSource7 
#define USART6_USART					USART6
#define USART6_USART_IRQHandler			USART6_IRQHandler
#define	USART6_USART_IRQn				USART6_IRQn



void usart6Init(void);
void usart6Stop(void);
void usart6SendChar(const uint8_t data);
void usart6SendBuffer(const uint8_t *buffer, uint16_t count);
void usart6SendString(const uint8_t *s);

#endif





