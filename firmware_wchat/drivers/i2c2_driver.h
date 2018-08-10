


#ifndef _I2C2_DRIVER_H
#define _I2C2_DRIVER_H


#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>

//! software iic driver

#define I2C2_RCC_AHB1Periph			RCC_AHB1Periph_GPIOA
#define I2C2_GPIO					GPIOA

#define I2C2_GPIO_REG_SCL           GPIOA
#define	I2C2_GPIO_PIN_SCL			GPIO_Pin_5		//! PA.05
#define I2C2_GPIO_REG_SDA           GPIOA
#define	I2C2_GPIO_PIN_SDA			GPIO_Pin_4		//! PA.04
#define I2C2_GPIO_REG_CS            GPIOA
#define	I2C2_GPIO_PIN_CS			GPIO_Pin_6		//! PA.06

#define I2C2_SCL_H()                GPIO_SetBits(I2C2_GPIO_REG_SCL, I2C2_GPIO_PIN_SCL)				
#define I2C2_SCL_L()                GPIO_ResetBits(I2C2_GPIO_REG_SCL, I2C2_GPIO_PIN_SCL)				

#define I2C2_SDA_H()                GPIO_SetBits(I2C2_GPIO_REG_SDA, I2C2_GPIO_PIN_SDA)			
#define I2C2_SDA_L()                GPIO_ResetBits(I2C2_GPIO_REG_SDA, I2C2_GPIO_PIN_SDA)			

#define I2C2_SDA_R()                GPIO_ReadInputDataBit(I2C2_GPIO_REG_SDA, I2C2_GPIO_PIN_SDA)

void I2C2_SDA_IN(void);
void I2C2_SDA_OUT(void);

void i2c2Init(void);
void i2c2Start(void);
void i2c2Stop(void);
void i2c2SendByte(uint8_t byte);
uint8_t i2c2ReadByte(void);
bool i2c2WaitAck(void);
void i2c2Ack(void);
void i2c2NAck(void);


bool i2c2WriteNByte(uint8_t addr, uint8_t *pBuf, uint8_t n);
bool i2c2ReadNByte(uint8_t addr, uint8_t *pBuf, uint8_t n);


#endif


