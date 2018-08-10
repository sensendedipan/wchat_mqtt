


#ifndef _W25QX_DRIVER_H
#define _W25QX_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>



#define W25Q80 	0XEF13 	//W25Q80  ID  0XEF13
#define W25Q16 	0XEF14  //W25Q16  ID  0XEF14
#define W25Q32 	0XEF15  //W25Q32  ID  0XEF15
#define W25Q64 	0XEF16  //W25Q64  ID  0XEF16
#define W25Q128	0XEF17  //W25Q128 ID  0XEF17


//! flash/W25Q128
#define FLASH_PAGESIZE   		256
#define FLASH_ID                0xEF17

#define W25Q_WriteEnable		0x06 
#define W25Q_WriteDisable		0x04 
#define W25Q_ReadStatusReg		0x05 
#define W25Q_WriteStatusReg		0x01 
#define W25Q_ReadData			0x03 
#define W25Q_FastReadData		0x0B 
#define W25Q_FastReadDual		0x3B 
#define W25Q_PageProgram		0x02 
#define W25Q_BlockErase64K  	0xD8 
#define W25Q_BlockErase32K  	0x52 
#define W25Q_SectorErase		0x20 
#define W25Q_ChipErase			0xC7 
#define W25Q_PowerDown			0xB9 
#define W25Q_ReleasePowerDown	0xAB 
#define W25Q_DeviceID			0xAB 
#define W25Q_ManufactDeviceID	0x90 
#define W25Q_JedecDeviceID		0x9F 

#define FLASH_GPIO_REG					GPIOB
#define	FLASH_GPIO_PIN_CS				GPIO_Pin_6		//! PB.06
#define FLASH_RCC_AHB1Periph			RCC_AHB1Periph_GPIOB


void spi1FlashInit(void);
void spi1FlashWakeUp(void);
static uint8_t spi1FlashReadStatusReg(void);
static void spi1FlashWriteStatusReg(uint8_t data);
static void spi1FlashWaitBusy(void);
static void spi1FlashWriteEnable(void);
static void spi1FlashWriteDisable(void);
void spi1FlashRead(uint8_t *buffer, uint32_t readAddr, uint16_t n);
void spi1FlashWritePage(uint8_t *buffer, uint32_t writeAddr, uint16_t n);
void spi1FlashWriteBuffer(uint8_t* pBuffer, uint32_t writeAddr, uint16_t numByteToWrite);
void spi1FlashEraseChip(void);
void spi1FlashEraseSector(uint32_t addrSect);
void spi1FlashEraseBlock32(uint32_t addrBlock32);
void spi1FlashEraseBlock64(uint32_t addrBlock64);
uint16_t spi1FlashReadID(void);



#endif

