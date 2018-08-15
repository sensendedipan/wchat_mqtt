
#ifndef _FIFORX_H
#define _FIFORX_H

#include <stm32f4xx.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>



//! fifo buffer size, should be n^2 value 
#define FIFO_RX_BUF_SIZE	16384 //! 16K 最大只能定义16K
#define FIFO_RX_BUF_MASK	FIFO_RX_BUF_SIZE - 1

#define OVREFLOW_FLAG_RX2	0x01
#define EMPTY_FLAG_RX2		0x02


uint16_t fifoRxPushBuf(uint8_t* data, uint16_t n);    //! write n data to buffer 
uint16_t fifoRxPopBuf(uint8_t* out_buf, uint16_t n);  //! read n data from buffer 
uint16_t fifoRxGetBufDataCount(void);                 //! get unread data count in buffer 
void fifoRxFlushBuf(void);                            //! flush buffer 
void fifoRxLookBuf(uint8_t* out_buf);                 //! look data in buffer, but not read out 


#endif



