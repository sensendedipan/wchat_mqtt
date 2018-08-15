
#include "eeprom.h"
#include "board.h"
#include "usart4_driver.h"




/**
  * @brief  usart4 init/gps
  * @param	baud
  * @retval None
  */
void usart4Init(void) 
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	USART_DeInit(USART4_USART);

	RCC_AHB1PeriphClockCmd(USART4_RCC_AHB1Periph, ENABLE);
	RCC_APB1PeriphClockCmd(USART4_RCC_APB1Periph_USART4, ENABLE);
    
    GPIO_PinAFConfig(USART4_GPIO, USART4_GPIO_PIN_SOURCE_TX, GPIO_AF_UART4);
    GPIO_PinAFConfig(USART4_GPIO, USART4_GPIO_PIN_SOURCE_RX, GPIO_AF_UART4);
	
	GPIO_InitStructure.GPIO_Pin = USART4_GPIO_PIN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(USART4_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART4_GPIO_PIN_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART4_GPIO, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 921600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART4_USART, &USART_InitStructure);
    USART_GetFlagStatus(USART4_USART, USART_FLAG_TC); 
	USART_ITConfig(USART4_USART, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART4_USART, ENABLE);
	USART_ClearFlag(USART4_USART, USART_FLAG_TC);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART4_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
 


void usart4Stop(void) 
{
	USART_DeInit(USART4_USART);	
}



void usart4SendChar(const uint8_t data) 
{  
    USART_SendData(USART4_USART, data);  
    while (USART_GetFlagStatus(USART4_USART, USART_FLAG_TC) == RESET){}  
} 



void usart4SendBuffer(const uint8_t *buffer, uint16_t count) 
{
	while (count--) {    
		usart4SendChar(*buffer);    
		buffer++;    
	}
}



//void USART4_USART_IRQHandler() 
//{
//	uint8_t data;
//	if (USART_GetITStatus(USART4_USART, USART_IT_RXNE) != RESET) { 
//		data = USART_ReceiveData(USART4_USART);
//	}
//}


/**
  * @brief  use usrt4 as printf() 
  * @note
  */
#if 1
int fputc(int ch, FILE *f)
{
    USART_SendData(USART4_USART, (unsigned char) ch);  
    while (USART_GetFlagStatus(USART4_USART, USART_FLAG_TC) == RESET){} 
	return (ch);
} 
#endif

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
