
#ifndef _DMA_DRIVER_H
#define _DMA_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>

#define DMA_STREAM          DMA2_Stream0
#define DMA_CHANNEL         DMA_Channel_4
#define DMA_STREAM_CLOCK    RCC_AHB1Periph_DMA2
#define DMA_FLAG_TCIF       DMA_FLAG_TCIF0

#define BUFFER_SIZE         128
#define TIMEOUT_MAX         10000 /* Maximum timeout value */




#endif
