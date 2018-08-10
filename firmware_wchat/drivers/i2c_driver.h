


#ifndef _I2C_DRIVER_H
#define _I2C_DRIVER_H


#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>

//! software iic driver
#define E2PROM_WR	0		/* Ð´¿ØÖÆbit */
#define E2PROM_RD	1		/* ¶Á¿ØÖÆbit */

#define I2C_RCC_AHB1Periph			RCC_AHB1Periph_GPIOB
#define I2C_GPIO					GPIOB

#define I2C_GPIO_REG_SCL            GPIOB
#define	I2C_GPIO_PIN_SCL			GPIO_Pin_6		//! PB.06
#define I2C_GPIO_REG_SDA            GPIOB
#define	I2C_GPIO_PIN_SDA			GPIO_Pin_7		//! PB.07

#define I2C_SCL_H()                 GPIO_SetBits(I2C_GPIO_REG_SCL, I2C_GPIO_PIN_SCL)				
#define I2C_SCL_L()                 GPIO_ResetBits(I2C_GPIO_REG_SCL, I2C_GPIO_PIN_SCL)				

#define I2C_SDA_H()                 GPIO_SetBits(I2C_GPIO_REG_SDA, I2C_GPIO_PIN_SDA)			
#define I2C_SDA_L()                 GPIO_ResetBits(I2C_GPIO_REG_SDA, I2C_GPIO_PIN_SDA)			

#define I2C_SDA_R()                 GPIO_ReadInputDataBit(I2C_GPIO_REG_SDA, I2C_GPIO_PIN_SDA)

static void I2C_SDA_IN(void);
static void I2C_SDA_OUT(void);

void i2cInit(void);
void i2cStart(void);
void i2cStop(void);
void i2cSendByte(uint8_t byte);
uint8_t i2cReadByte(void);
bool i2cWaitAck(void);
void i2cAck(void);
void i2cNAck(void);


#endif


