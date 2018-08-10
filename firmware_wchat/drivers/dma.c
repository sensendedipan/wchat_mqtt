
#include "dma.h"

static uint8_t RX_BUFFER[BUFFER_SIZE];

void dmaInit(void) 
{
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
    DMA_DeInit(DMA_STREAM);
    
    while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE){} //!等待DMA可配置 
        
    DMA_InitStructure.DMA_Channel = DMA_CHANNEL;                            //! DMA 数据流通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;       //! 源数据地址        
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RX_BUFFER;            //! 目标地址        
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 //! 外设到内存模式
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;                         //! 数据数目
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //! 禁止外设地址自增功能
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //! 使能内存地址自增功能
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //!
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //!
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //!
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     //!
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //!
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;       //! 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             //! 
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     //! 
    DMA_Init(DMA_STREAM, &DMA_InitStructure);                               //!

    DMA_Cmd(DMA_STREAM, ENABLE);                                            //!

    USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);                            //!

}    
    
   