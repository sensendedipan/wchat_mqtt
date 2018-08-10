

#ifndef _SPI1_DRIVER_H
#define _SPI1_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>




//! spi1/flash(W25QXX)

#define SPI1_RCC_AHB1Periph				RCC_AHB1Periph_GPIOB
#define SPI1_RCC_APB2Periph_SPI1		RCC_APB2Periph_SPI1
#define	SPI1_GPIO						GPIOB
#define	SPI1_GPIO_PIN_SCK				GPIO_Pin_3	//! PB.03
#define	SPI1_GPIO_PIN_MISO				GPIO_Pin_4	//! PB.04
#define SPI1_GPIO_PIN_MOSI				GPIO_Pin_5	//! PB.05
#define	SPI1_GPIO_BaudRatePrescaler		SPI_BaudRatePrescaler_4 



void spi1Init(void);
uint8_t spi1ReadWriteByte(uint8_t data);




#endif


