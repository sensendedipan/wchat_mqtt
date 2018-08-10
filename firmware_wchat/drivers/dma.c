
#include "dma.h"

static uint8_t RX_BUFFER[BUFFER_SIZE];

void dmaInit(void) 
{
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
    DMA_DeInit(DMA_STREAM);
    
    while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE){} //!�ȴ�DMA������ 
        
    DMA_InitStructure.DMA_Channel = DMA_CHANNEL;                            //! DMA ������ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;       //! Դ���ݵ�ַ        
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RX_BUFFER;            //! Ŀ���ַ        
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 //! ���赽�ڴ�ģʽ
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;                         //! ������Ŀ
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //! ��ֹ�����ַ��������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //! ʹ���ڴ��ַ��������
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
    
   