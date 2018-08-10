


#include "board.h"


void spiInit(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI2_RCC_APB2Periph, ENABLE);
	RCC_APB1PeriphClockCmd(SPI2_RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = SPI2_GPIO_PIN_SCK | SPI2_GPIO_PIN_MISO | SPI2_GPIO_PIN_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI2_GPIO_BaudRatePrescaler; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);  
	spiReadWriteByte(0xFF);	
	
	printf("ITSC: spi initialization OK ! \n");
}


/**
  * @brief  Reads or write a byte for the FLASH
  * @param  data : 
  * @retval 
  */
uint8_t spiReadWriteByte(uint8_t data)
{		
	uint8_t retry = 0;	
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {
		retry++;
		if (retry > 200) return 0;
	}			  
	//! send byte through the SPI peripheral 
	SPI_I2S_SendData(SPI2, data); 
	retry = 0; 
	//! wait to receive a byte 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {
		retry++;
		if (retry > 200) return 0;
	}	  						    
	//! return the byte read from the SPI bus 
	return SPI_I2S_ReceiveData(SPI2); 		    
}



