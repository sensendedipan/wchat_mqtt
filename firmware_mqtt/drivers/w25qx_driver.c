



#include "w25qx_driver.h"
#include "spi1_driver.h"

/* W25Q128  = 128Mbit = 16M-byte = 256 Blocks = 4096 Sectors = */
/* W25Q16   = 16Mbit = 2M-byte = 32 Blocks = 512 Sectors = 8192 Pages = 2097152 Bytes
 * a page   = 256-bytes
 * a sector = 16pages = 4K-bytes 
 * a block  = 16sectors = 256pages = 64K-bytes

 *   block_0 -- block_31:   32blocks
 *   0x000000h-0x1fffffh 共32个block 每个占64K
 *   block_0 : 0x000000  -  0x00ffff
 *   block_1 : 0x010000  -  0x01ffff
 *   block_2 : 0x020000  -  0x02ffff
 *   block_3 : 0x030000  -  0x03ffff 
 *   block_4 : 0x040000  -  0x04ffff
 *   block_5 : 0x050000  -  0x05ffff 
 *   block_6 : 0x060000  -  0x06ffff 
 *   block_7 : 0x070000  -  0x07ffff
 *   block_8 : 0x080000  -  0x08ffff 
 *   block_9 : 0x090000  -  0x06ffff 
 *   block_10: 0x0A0000  -  0x07ffff
 *   block_11: 0x0B0000  -  0x08ffff     
 *                .
 */
 

#define FLASH_DESELECT()        GPIO_SetBits(FLASH_GPIO_REG,   FLASH_GPIO_PIN_CS)
#define FLASH_SELECT()          GPIO_ResetBits(FLASH_GPIO_REG, FLASH_GPIO_PIN_CS)



/**
  * @brief  initialize the flash's cs pin
  */
void spi1FlashInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(FLASH_RCC_AHB1Periph, ENABLE);
	GPIO_InitStructure.GPIO_Pin = FLASH_GPIO_PIN_CS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(FLASH_GPIO_REG, &GPIO_InitStructure);

	FLASH_DESELECT();
	
	if (spi1FlashReadID() == FLASH_ID) {
		printf("huayun: w25q128 initialization OK ! \n");
		
	} else {
		printf("huayun: w25q128 ID %X initialization Failed ! \n", spi1FlashReadID());
	}
}


/**
  * @brief  wake up 
  * @note    
  */
void spi1FlashWakeUp(void)   
{
    FLASH_SELECT();
    spi1ReadWriteByte(W25Q_ReleasePowerDown);
    FLASH_DESELECT();    
} 



/**
  * @brief  power down 
  * @note    
  */
void spi1FlashPowerDown(void)   
{
    FLASH_SELECT();
    spi1ReadWriteByte(W25Q_PowerDown);
    FLASH_DESELECT();    
}


/**
  * @brief  Read the status registers of W25Q16
  * @note   BITS    9    8    7    6    5    4    3    2    1    0  
  * @note   QE SPR1 SPR0  SEC   TB  BP2  BP1  BP0  WEL  BUSY
  * @note   SPR:  默认0， 状态寄存器保护位，配合WP使用
  * @note   TB:   BP2,BP1,BP0:FLASH区域写保护设置
  * @note   WEL： 写使能锁定
  * @note   BUSY: 忙标记位(1: 忙; 0:空闲)
  */
static uint8_t spi1FlashReadStatusReg(void) 
{  
	uint8_t byte = 0;   
	FLASH_SELECT();                           
	spi1ReadWriteByte(W25Q_ReadStatusReg);      
	byte = spi1ReadWriteByte(0Xff);              
	FLASH_DESELECT();                               
	return byte;   
} 


/**
  * @brief  write the status registers of W25Q16
  * @param  data:
  * @note   only SPR TB BP2 BP1 BP0 can be writen into
  */
static void spi1FlashWriteStatusReg(uint8_t data) 
{
	FLASH_SELECT();                            
	spi1ReadWriteByte(W25Q_ReadStatusReg);     
	spi1ReadWriteByte(data);               
	FLASH_DESELECT();
}


/**
  * @brief  wait for not busy
  */
static void spi1FlashWaitBusy(void) 
{   
	while((spi1FlashReadStatusReg()&0x01)==0x01);
}


/**
  * @brief  spi write enable
  * @note   set WEL 
  */
static void spi1FlashWriteEnable(void) 
{
	FLASH_SELECT();                            
    spi1ReadWriteByte(W25Q_WriteEnable);     
	FLASH_DESELECT();                          	      
} 



/**
  * @brief  spi write disable
  * @note   clr WEL 
  */
static void spi1FlashWriteDisable(void) 
{
	FLASH_SELECT();                            
    spi1ReadWriteByte(W25Q_WriteDisable);     
	FLASH_DESELECT();                          	      
}


/**
  * @brief  read ID
  * @note   W25Q80		0xEF13
  * @note   W25Q16		0xEF14
  * @note   W25Q32		0xEF15
  * @note   W25Q64		0xEF16
  * @note   W25Q128		0xEF17
  */
uint16_t spi1FlashReadID(void) 
{
	uint16_t temp = 0;	  
	FLASH_SELECT();				    
	spi1ReadWriteByte(W25Q_ManufactDeviceID); 
	spi1ReadWriteByte(0x00); 	    
	spi1ReadWriteByte(0x00); 	    
	spi1ReadWriteByte(0x00); 	 	
	temp |= spi1ReadWriteByte(0xFF)<<8;  
	temp |= spi1ReadWriteByte(0xFF);	 
	FLASH_DESELECT();				    
	return temp;
}


/**
  * @brief  read n bytes flash data from readaddr to pbuffer
  * @param  buffer  : address for hold data
  * @param  readAddr: address for read data
  * @param  n       : n bytes to read (max is 65535)
  */
void spi1FlashRead(uint8_t *buffer, uint32_t readAddr, uint16_t n) 
{ 
 	uint16_t i;    												    
	FLASH_SELECT();                              
    spi1ReadWriteByte(W25Q_ReadData);            
    spi1ReadWriteByte((uint8_t)((readAddr)>>16));   
    spi1ReadWriteByte((uint8_t)((readAddr)>>8));   
    spi1ReadWriteByte((uint8_t)readAddr);   
    for (i = 0; i < n; i++) { 
        buffer[i]=spi1ReadWriteByte(0xFF);   
    }
	FLASH_DESELECT();                            	      
}


/**
  * @brief  write n bytes to flash (n must be less than 256)
  * @param  buffer   : address of data where it comes from
  * @param  writeAddr: address of date need to be writen to 
  * @param  n        : n bytes to be writen(less than 256, for a page's size is 256Bytes)
  */
void spi1FlashWritePage(uint8_t *buffer, uint32_t writeAddr, uint16_t n) 
{
 	uint16_t i;  
    spi1FlashWriteEnable();             
	FLASH_SELECT();                          
    spi1ReadWriteByte(W25Q_PageProgram);        
    spi1ReadWriteByte((u8)((writeAddr)>>16)); 
    spi1ReadWriteByte((u8)((writeAddr)>>8));   
    spi1ReadWriteByte((u8)writeAddr);   
    for (i = 0; i < n; i++) {
		spi1ReadWriteByte(buffer[i]);
	}
	FLASH_DESELECT();                            
	spi1FlashWaitBusy();					   
} 



 /**
  * @brief  
  * @param	
  * @param  
  * @param 
  * @retval
  */
void spi1FlashWriteBuffer(uint8_t* pBuffer, uint32_t writeAddr, uint16_t numByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = writeAddr % FLASH_PAGESIZE;

    count = FLASH_PAGESIZE - Addr;	
    NumOfPage =  numByteToWrite / FLASH_PAGESIZE;
    NumOfSingle = numByteToWrite % FLASH_PAGESIZE;

    if (Addr == 0) {

        if (NumOfPage == 0) {
            spi1FlashWritePage(pBuffer, writeAddr, numByteToWrite);
            
        } else {
            while (NumOfPage--) {
                spi1FlashWritePage(pBuffer, writeAddr, FLASH_PAGESIZE);
                writeAddr +=  FLASH_PAGESIZE;
                pBuffer += FLASH_PAGESIZE;
            }
            spi1FlashWritePage(pBuffer, writeAddr, NumOfSingle);
        }
        
    } else {
        if (NumOfPage == 0) {
            if (NumOfSingle > count) {
                temp = NumOfSingle - count;

                spi1FlashWritePage(pBuffer, writeAddr, count);
                writeAddr +=  count;
                pBuffer += count;

                spi1FlashWritePage(pBuffer, writeAddr, temp);
                
            } else  {				
                spi1FlashWritePage(pBuffer, writeAddr, numByteToWrite);
            }
            
        } else {
            numByteToWrite -= count;
            NumOfPage =  numByteToWrite / FLASH_PAGESIZE;
            NumOfSingle = numByteToWrite % FLASH_PAGESIZE;

            spi1FlashWritePage(pBuffer, writeAddr, count);
            writeAddr +=  count;
            pBuffer += count;

            while (NumOfPage--) {
                spi1FlashWritePage(pBuffer, writeAddr, FLASH_PAGESIZE);
                writeAddr +=  FLASH_PAGESIZE;
                pBuffer += FLASH_PAGESIZE;
            }

            if (NumOfSingle != 0) {
                spi1FlashWritePage(pBuffer, writeAddr, NumOfSingle);
            }
        }
    }
}



/**
  * @brief  erase the whole chip
  * @note   W25Q16: 25s
  * @note   W25Q32: 40s
  * @note   W25Q64: 40s
  */
void spi2FlashEraseChip(void) 
{                                             
    spi1FlashWriteEnable();                 
    spi1FlashWaitBusy();   
  	FLASH_SELECT();                             
    spi1ReadWriteByte(W25Q_ChipErase);        
	FLASH_DESELECT();                              	      
	spi1FlashWaitBusy();   				   
} 



/**
  * @brief  erase a sector
  * @param  addrSect: 0 ~ 511 for W25Q16  
  * @note   each sector is 4096 bytes 
  * @note   erase a sector needs at least 150ms
  */
void spi1FlashEraseSector(uint32_t addrSect) 
{                                             
    spi1FlashWriteEnable();
    spi1FlashWaitBusy();   
  	FLASH_SELECT();                          
    spi1ReadWriteByte(W25Q_SectorErase);       
    spi1ReadWriteByte((addrSect & 0xFF0000) >> 16);
    spi1ReadWriteByte((addrSect & 0xFF00) >> 8);
    spi1ReadWriteByte(addrSect & 0xFF);  
	FLASH_DESELECT();                            	      
    spi1FlashWaitBusy();   				  
} 


/**
  * @brief  erase a 32K block
  * @param  addrBlock32: 0 ~ 31 for W25Q16 
  * @note   each block is 32K bytes
  */
void spi1FlashEraseBlock32(uint32_t addrBlock32) 
{                                             
	addrBlock32 *= 32768;
    spi1FlashWriteEnable();
    spi1FlashWaitBusy();   
  	FLASH_SELECT();                          
    spi1ReadWriteByte(W25Q_BlockErase32K);       
    spi1ReadWriteByte((uint8_t)((addrBlock32)>>16));  
    spi1ReadWriteByte((uint8_t)((addrBlock32)>>8));   
    spi1ReadWriteByte((uint8_t)addrBlock32);  
	FLASH_DESELECT();                            	      
    spi1FlashWaitBusy();   				  
} 


/**
  * @brief  erase a 64K block
  * @param  addrBlock64: 0 ~ 15 for W25Q16
  */
void spi1FlashEraseBlock64(uint32_t addrBlock64) 
{                                             
	addrBlock64 *= 65536;
    spi1FlashWriteEnable();
    spi1FlashWaitBusy();   
  	FLASH_SELECT();                          
    spi1ReadWriteByte(W25Q_BlockErase64K);       
    spi1ReadWriteByte((uint8_t)((addrBlock64)>>16));  
    spi1ReadWriteByte((uint8_t)((addrBlock64)>>8));   
    spi1ReadWriteByte((uint8_t)addrBlock64);  
	FLASH_DESELECT();                            	      
    spi1FlashWaitBusy();   				  
} 





























