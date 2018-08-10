


#ifndef _USART3_DRIVER_H
#define _USART3_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"


//! usart3/chz

#define RS485_RCC_AHB1Periph			RCC_AHB1Periph_GPIOB
#define	RS485_RCC_APB1Periph_USART3	    RCC_APB1Periph_USART3
#define	RS485_GPIO						GPIOB
#define	RS485_GPIO_PIN_TX				GPIO_Pin_10	 //! PB.10
#define	RS485_GPIO_PIN_RX				GPIO_Pin_11  //! PB.11
#define	RS485_GPIO_PIN_SOURCE_TX		GPIO_PinSource10 
#define	RS485_GPIO_PIN_SOURCE_RX		GPIO_PinSource11 
#define RS485_USART					    USART3
#define RS485_USART_IRQHandler			USART3_IRQHandler
#define	RS485_USART_IRQn				USART3_IRQn
                                        
#define RS485_RW_REG                    GPIOD
#define RS485_RW_PIN                    GPIO_Pin_14
#define RS485_RW_RCC_AHB1Periph         RCC_AHB1Periph_GPIOD

#define RS485_RW_H()                    GPIO_SetBits(RS485_RW_REG, RS485_RW_PIN)			
#define RS485_RW_L()                    GPIO_ResetBits(RS485_RW_REG, RS485_RW_PIN)

void rs485Init(void);
void rs485Stop(void);
void rs485SendChar(const uint8_t data);
void rs485SendBuffer(const uint8_t *buffer, uint16_t count);
void rs485SendString(const uint8_t *s); 



typedef enum rs485_read_state_t
{
    RS485_STATE_READY = 0,
    RS485_STATE_WAIT_PMU_ACK,
    RS485_STATE_WAIT_PSD_ACK,

} rs485_read_state_t;


#endif
