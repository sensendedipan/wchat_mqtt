


#ifndef _USART1_DRIVER_H
#define _USART1_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>


//! usart1

#define USART1_RCC_AHB1Periph			RCC_AHB1Periph_GPIOA
#define	USART1_RCC_APB2Periph_USART1	RCC_APB2Periph_USART1
#define	USART1_GPIO						GPIOA
#define	USART1_GPIO_PIN_TX				GPIO_Pin_9	//! PA.09
#define	USART1_GPIO_PIN_RX				GPIO_Pin_10 //! PA.10
#define	USART1_GPIO_PIN_SOURCE_TX		GPIO_PinSource9 
#define	USART1_GPIO_PIN_SOURCE_RX		GPIO_PinSource10 
#define USART1_USART					USART1
#define USART1_USART_IRQHandler			USART1_IRQHandler
#define	USART1_USART_IRQn				USART1_IRQn


static void USART_CRIS_ENTER(void);
static void USART_CRIS_EXIT(void);

void usart1Init(void);
void usart1Stop(void);
void usart1SendChar(const uint8_t data);
void usart1SendBuffer(const uint8_t *buffer, uint16_t count);
void usart1SendString(const uint8_t *s);

//! dma
#define DMA_STREAM_RX               DMA2_Stream2
#define DMA_STREAM_TX               DMA2_Stream7
#define DMA_CHANNEL_RX              DMA_Channel_4
#define DMA_CHANNEL_TX              DMA_Channel_4
#define DMA_STREAM_CLOCK            RCC_AHB1Periph_DMA2
#define DMA_FLAG_TCIF               DMA_FLAG_TCIF2


#define DMA_RX_BUFFER_SIZE          4096  //! 4K
#define DMA_TX_BUFFER_SIZE          1024


void usart1DmaSendBuffer(const uint8_t *buffer, uint16_t count);












#endif
