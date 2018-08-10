

#ifndef _SPI2_DRIVER_H
#define _SPI2_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>


//! spi2/W5500

#define SPI2_RCC_AHB1Periph				RCC_AHB1Periph_GPIOB
#define SPI2_RCC_APB1Periph_SPI2		RCC_APB1Periph_SPI2
#define	SPI2_GPIO						GPIOB
#define	SPI2_GPIO_PIN_SCK				GPIO_Pin_13	//! PB.13
#define	SPI2_GPIO_PIN_MISO				GPIO_Pin_14	//! PB.14
#define SPI2_GPIO_PIN_MOSI				GPIO_Pin_15	//! PB.15
#define	SPI2_GPIO_PIN_SOURCE_SCK		GPIO_PinSource13	//! PB.13
#define	SPI2_GPIO_PIN_SOURCE_MISO		GPIO_PinSource14	//! PB.14
#define SPI2_GPIO_PIN_SOURCE_MOSI		GPIO_PinSource15	//! PB.15
#define	SPI2_GPIO_BaudRatePrescaler		SPI_BaudRatePrescaler_2 



void spi2Init(void);
uint8_t spi2ReadWriteByte(uint8_t data);


#endif


