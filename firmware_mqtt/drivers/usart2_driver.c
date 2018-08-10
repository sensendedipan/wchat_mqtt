


#include "board.h"
#include "usart2_driver.h"


/**
  * @brief  usart2 init/att7053bu
  * @param	baud
  * @retval None
  */
void usart2Init(void) 
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;	
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	USART_DeInit(USART2_USART);
	RCC_AHB1PeriphClockCmd(USART2_RCC_AHB1Periph, ENABLE);
	RCC_APB1PeriphClockCmd(USART2_RCC_APB1Periph_USART2, ENABLE);

    GPIO_PinAFConfig(USART2_GPIO, USART2_GPIO_PIN_SOURCE_TX, GPIO_AF_USART2);
    GPIO_PinAFConfig(USART2_GPIO, USART2_GPIO_PIN_SOURCE_RX, GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Pin = USART2_GPIO_PIN_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART2_GPIO_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;  

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
   
	USART_Init(USART2_USART, &USART_InitStructure);
    USART_GetFlagStatus(USART2_USART, USART_FLAG_TC); 
	//USART_ITConfig(USART2_USART, USART_IT_IDLE, ENABLE); //! Ê¹ÄÜ¿ÕÏÐÖÐ¶Ï
    USART_ITConfig(USART2_USART, USART_IT_RXNE, ENABLE); //! 
    USART_ITConfig(USART2_USART, USART_IT_TC,   DISABLE);//! 
	USART_Cmd(USART2_USART, ENABLE);
	USART_ClearFlag(USART2_USART, USART_FLAG_TC);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 


void usart2Stop(void) 
{
	USART_DeInit(USART2_USART);	
}



void usart2SendChar(const uint8_t data) 
{  
    USART_SendData(USART2_USART, data);  
    while (USART_GetFlagStatus(USART2_USART, USART_FLAG_TXE) == RESET){}  //TC
} 



void usart2SendBuffer(const uint8_t *buffer, uint16_t count) 
{
	while (count--) {    
		usart2SendChar(*buffer);    
		buffer++;    
	}
    while (USART_GetFlagStatus(USART2_USART, USART_FLAG_TC) == RESET){}
}



void usart2SendString(const uint8_t *s) 
{
	
	while (*s) {    
		while(USART_GetFlagStatus(USART2_USART, USART_FLAG_TC) == RESET); 
		USART_SendData(USART2_USART , *s++); 
	}
}



 

 
 
 
 
 
 
 
 
 
 
 
