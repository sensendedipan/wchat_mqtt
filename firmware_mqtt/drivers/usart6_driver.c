
#include "usart6_driver.h"



/**
  * @brief  usart6 init/user config
  * @param	baud
  * @retval None
  */
void usart6Init(void) 
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
		
	USART_DeInit(USART6_USART);
    RCC_AHB1PeriphClockCmd(USART6_RCC_AHB1Periph, ENABLE);
    RCC_APB2PeriphClockCmd(USART6_RCC_APB2Periph_USART6, ENABLE);
    
    GPIO_PinAFConfig(USART6_GPIO, USART6_GPIO_PIN_SOURCE_TX, GPIO_AF_USART6);
    GPIO_PinAFConfig(USART6_GPIO, USART6_GPIO_PIN_SOURCE_RX, GPIO_AF_USART6);
	
	GPIO_InitStructure.GPIO_Pin = USART6_GPIO_PIN_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(USART6_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART6_GPIO_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART6_GPIO, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6_USART, &USART_InitStructure);
    USART_GetFlagStatus(USART6_USART, USART_FLAG_TC); 
	USART_ITConfig(USART6_USART, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART6_USART, ENABLE);
	USART_ClearFlag(USART6_USART, USART_FLAG_TC);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART6_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	printf("huayun: usart6 (115200) initialization OK ! \n");
}
 

void usart6Stop(void) 
{
	
	USART_DeInit(USART6_USART);	
}


void usart6SendChar(const uint8_t data) 
{  
	
    USART_SendData(USART6_USART, data);  
    while (USART_GetFlagStatus(USART6_USART, USART_FLAG_TC) == RESET){}  
} 


void usart6SendBuffer(const uint8_t *buffer, uint16_t count) 
{
	
	while (count--) {    
		usart6SendChar(*buffer);    
		buffer++;    
	}
}


void usart6SendString(const uint8_t *s) 
{
	
	while (*s) {    
		while(USART_GetFlagStatus(USART6_USART, USART_FLAG_TC) == RESET); 
		USART_SendData(USART6_USART , *s++); 
	}
}





  