


#include "spi1_driver.h"


void spi1Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(SPI1_RCC_AHB1Periph, ENABLE);
	RCC_APB2PeriphClockCmd(SPI1_RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = SPI1_GPIO_PIN_SCK | SPI1_GPIO_PIN_MISO | SPI1_GPIO_PIN_MOSI;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS  = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI1_GPIO_BaudRatePrescaler; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);  	
	
	printf("huayun: spi1 initialization OK ! \n");
}


/**
  * @brief  Reads or write a byte for the FLASH
  * @param  data : 
  * @retval 
  */
uint8_t spi1ReadWriteByte(uint8_t data)
{		
	uint8_t retry = 0;	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) {
		retry++;
		if (retry > 200) return 0;
	}			  
	//! send byte through the SPI peripheral 
	SPI_I2S_SendData(SPI1, data); 
	retry = 0; 
	//! wait to receive a byte 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
		retry++;
		if (retry > 200) return 0;
	}	  						    
	//! return the byte read from the SPI bus 
	return SPI_I2S_ReceiveData(SPI1); 		    
}



