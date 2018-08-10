

#include "board.h"
#include "usart3_driver.h"

/**
  * @brief  usart3 chz
  * @param	
  * @retval 
  */
void rs485Init(void) 
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;	
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	USART_DeInit(RS485_USART);
	RCC_AHB1PeriphClockCmd(RS485_RCC_AHB1Periph | RS485_RW_RCC_AHB1Periph, ENABLE);
	RCC_APB1PeriphClockCmd(RS485_RCC_APB1Periph_USART3, ENABLE);

    GPIO_PinAFConfig(RS485_GPIO, RS485_GPIO_PIN_SOURCE_TX, GPIO_AF_USART3);
    GPIO_PinAFConfig(RS485_GPIO, RS485_GPIO_PIN_SOURCE_RX, GPIO_AF_USART3);
	
	GPIO_InitStructure.GPIO_Pin = RS485_GPIO_PIN_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(RS485_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RS485_GPIO_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(RS485_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
    GPIO_InitStructure.GPIO_Pin = RS485_RW_PIN;
    GPIO_Init(RS485_RW_REG, &GPIO_InitStructure);    

	USART_InitStructure.USART_BaudRate = 9600;    
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(RS485_USART, &USART_InitStructure);
    USART_GetFlagStatus(RS485_USART, USART_FLAG_TC); 
	USART_ITConfig(RS485_USART, USART_IT_RXNE, ENABLE);
	USART_Cmd(RS485_USART, ENABLE);
	USART_ClearFlag(RS485_USART, USART_FLAG_TC);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = RS485_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 


void rs485Stop(void) 
{
	USART_DeInit(RS485_USART);	
}



void rs485SendChar(const uint8_t data) 
{  
    USART_SendData(RS485_USART, data);  
    while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TC) == RESET){}  
} 



void rs485SendBuffer(const uint8_t *buffer, uint16_t count) 
{
	while (count--) {    
		rs485SendChar(*buffer);    
		buffer++;    
	}
}



void rs485SendString(const uint8_t *s) 
{	
	while (*s) {    
		while(USART_GetFlagStatus(RS485_USART, USART_FLAG_TC) == RESET); 
		USART_SendData(RS485_USART , *s++); 
	}
}






 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
