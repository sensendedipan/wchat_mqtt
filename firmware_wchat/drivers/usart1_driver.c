
#include "board.h"
#include "usart1_driver.h"


static DMA_InitTypeDef DMA_InitStructureTx;
static DMA_InitTypeDef DMA_InitStructureRx;

static uint8_t RX_BUFFER[DMA_RX_BUFFER_SIZE];


/**
  * @brief  usart1 init/gprs
  * @param	baud
  * @retval None
  */
void usart1Init(void) 
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	USART_DeInit(USART1_USART);
    RCC_AHB1PeriphClockCmd(USART1_RCC_AHB1Periph, ENABLE);
	RCC_APB2PeriphClockCmd(USART1_RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_PinAFConfig(USART1_GPIO, USART1_GPIO_PIN_SOURCE_TX, GPIO_AF_USART1);
    GPIO_PinAFConfig(USART1_GPIO, USART1_GPIO_PIN_SOURCE_RX, GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_PIN_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART1_GPIO_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
    USART_OverSampling8Cmd(USART1_USART, ENABLE);
    USART_OneBitMethodCmd(USART1_USART, ENABLE);
    
    if (gEepromLnet.baud == 0) {
        gEepromLnet.baud = 8000000;
        
    } else if (gEepromLnet.baud > 10500000) {
        gEepromLnet.baud = 10500000;
    }
	USART_InitStructure.USART_BaudRate = gEepromLnet.baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1_USART, &USART_InitStructure);
    USART_GetFlagStatus(USART1_USART, USART_FLAG_TC); 
	USART_ITConfig(USART1_USART, USART_IT_IDLE, ENABLE); //! 使能空闲中断
    USART_ITConfig(USART1_USART, USART_IT_RXNE, DISABLE);//! 禁止接收完成中断
    USART_ITConfig(USART1_USART, USART_IT_TC,   DISABLE);//! 禁止发送完成中断
	USART_Cmd(USART1_USART, ENABLE);
	USART_ClearFlag(USART1_USART, USART_FLAG_TC);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
    
    DMA_DeInit(DMA_STREAM_RX);    
        
    DMA_InitStructureRx.DMA_Channel             = DMA_CHANNEL_RX;              //! DMA 数据流通道选择
    DMA_InitStructureRx.DMA_PeripheralBaseAddr  = (uint32_t)&USART1_USART->DR; //! 源数据地址        
    DMA_InitStructureRx.DMA_Memory0BaseAddr     = (uint32_t)RX_BUFFER;         //! 目标地址        
    DMA_InitStructureRx.DMA_DIR                 = DMA_DIR_PeripheralToMemory;  //! 外设到内存模式
    DMA_InitStructureRx.DMA_BufferSize          = DMA_RX_BUFFER_SIZE;          //! 数据数目
    DMA_InitStructureRx.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;   //! 禁止外设地址自增功能
    DMA_InitStructureRx.DMA_MemoryInc           = DMA_MemoryInc_Enable;        //! 使能内存地址自增功能
    DMA_InitStructureRx.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_Byte; //!
    DMA_InitStructureRx.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;     //!
    DMA_InitStructureRx.DMA_Mode                = DMA_Mode_Normal;             //!
    DMA_InitStructureRx.DMA_Priority            = DMA_Priority_High;           //!
    DMA_InitStructureRx.DMA_FIFOMode            = DMA_FIFOMode_Disable;        //!
    DMA_InitStructureRx.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull;  //! 
    DMA_InitStructureRx.DMA_MemoryBurst         = DMA_MemoryBurst_Single;      //! 
    DMA_InitStructureRx.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;  //! 
    DMA_Init(DMA_STREAM_RX, &DMA_InitStructureRx);                             //!
    DMA_Cmd(DMA_STREAM_RX, ENABLE);                                            //!
    USART_DMACmd(USART1_USART, USART_DMAReq_Rx, ENABLE);                       //!

#if 0
    DMA_DeInit(DMA_STREAM_TX);        
    DMA_InitStructureTx.DMA_Channel             = DMA_CHANNEL_TX;              //!
    DMA_InitStructureTx.DMA_PeripheralBaseAddr  = (uint32_t)&USART1_USART->DR; //!
    DMA_InitStructureTx.DMA_Memory0BaseAddr     = (uint32_t)TX_BUFFER;         //!
    DMA_InitStructureTx.DMA_DIR                 = DMA_DIR_MemoryToPeripheral;  //!
    DMA_InitStructureTx.DMA_BufferSize          = DMA_TX_BUFFER_SIZE;          //!                 
    DMA_InitStructureTx.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;   //!
    DMA_InitStructureTx.DMA_MemoryInc           = DMA_MemoryInc_Enable;        //!
    DMA_InitStructureTx.DMA_PeripheralDataSize  = DMA_MemoryDataSize_Byte;     //!
    DMA_InitStructureTx.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;     //!
    DMA_InitStructureTx.DMA_Mode                = DMA_Mode_Normal;             //!
    DMA_InitStructureTx.DMA_Priority            = DMA_Priority_High;           //!
    DMA_InitStructureTx.DMA_FIFOMode            = DMA_FIFOMode_Disable;        //!
    DMA_InitStructureTx.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull;  //!
    DMA_InitStructureTx.DMA_MemoryBurst         = DMA_MemoryBurst_Single;      //!
    DMA_InitStructureTx.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;
    DMA_Init(DMA_STREAM_TX, &DMA_InitStructureTx);                             //!
    DMA_Cmd(DMA_STREAM_TX, ENABLE);                                            //!
    USART_DMACmd(USART1_USART, USART_DMAReq_Tx, ENABLE);                       //!     
    DMA_Cmd(DMA_STREAM_TX, DISABLE); 
#endif    
}
 

static void USART_CRIS_ENTER(void)
{
	__set_PRIMASK(1);
}


static void USART_CRIS_EXIT(void)
{
	__set_PRIMASK(0);
}



void usart1Stop(void) 
{	
	USART_DeInit(USART1_USART);	
}


void usart1SendChar(const uint8_t data) 
{  	
    USART_SendData(USART1_USART, data);  
    while (USART_GetFlagStatus(USART1_USART, USART_FLAG_TXE) == RESET){}  
} 


void usart1SendBuffer(const uint8_t *buffer, uint16_t count) 
{	
	while (count--) {    
		usart1SendChar(*buffer);    
		buffer++;    
	}
    while (USART_GetFlagStatus(USART1_USART, USART_FLAG_TC) == RESET){}
}


void usart1SendString(const uint8_t *s) 
{	
	while (*s) {    
		while(USART_GetFlagStatus(USART1_USART, USART_FLAG_TC) == RESET); 
		USART_SendData(USART1_USART , *s++); 
	}
}


void usart1DmaSendBuffer(const uint8_t *buffer, uint16_t count)  
{  
//    DMA_Cmd(DMA_STREAM_TX, DISABLE);
//    DMA_DeInit(DMA_STREAM_TX); 
//    
//    memcpy(TX_BUFFER, buffer, count);
//    DMA_InitStructureTx.DMA_Memory0BaseAddr = (uint32_t)TX_BUFFER;
//    DMA_InitStructureTx.DMA_BufferSize = count;
//    DMA_Init(DMA_STREAM_TX, &DMA_InitStructureTx);
//    DMA_ITConfig(DMA_STREAM_TX, DMA_IT_TC, ENABLE);    
//    DMA_Cmd(DMA_STREAM_TX, ENABLE);  
} 


void USART1_USART_IRQHandler() 
{	
	static uint8_t data;

	if (USART_GetITStatus(USART1_USART, USART_IT_IDLE) != RESET) {        
        DMA_Cmd(DMA_STREAM_RX, DISABLE);  
        USART1_USART->SR;
        USART1_USART->DR;
        DMA_ClearFlag(DMA_STREAM_RX, DMA_FLAG_TCIF);

        uint16_t len = DMA_RX_BUFFER_SIZE - DMA_GetCurrDataCounter(DMA_STREAM_RX);
        if (len >= DMA_RX_BUFFER_SIZE) {
            systemReboot();
            
        } else if (len > 0) {
            fifoPushBuf(RX_BUFFER, len);
        }
        
        DMA_DeInit(DMA_STREAM_RX);
        DMA_InitStructureRx.DMA_Memory0BaseAddr = (uint32_t)RX_BUFFER;
        DMA_InitStructureRx.DMA_BufferSize = DMA_RX_BUFFER_SIZE;
        DMA_Init(DMA_STREAM_RX, &DMA_InitStructureRx);
        DMA_Cmd(DMA_STREAM_RX, ENABLE);    
	}
    USART_ClearITPendingBit(USART1_USART, USART_IT_IDLE | USART_IT_PE | USART_IT_FE | USART_IT_NE);
}

















  