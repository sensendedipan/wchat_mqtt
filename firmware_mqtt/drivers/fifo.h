
#ifndef _FIFO_H
#define _FIFO_H

#include <stm32f4xx.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>



//! fifo buffer size, should be n^2 value 
#define FIFO_BUF_SIZE	4096 //! 4K
#define FIFO_BUF_MASK	FIFO_BUF_SIZE - 1

#define OVREFLOW_FLAG1	0x01
#define EMPTY_FLAG1		0x02


uint16_t fifoPushBuf(uint8_t* data, uint16_t n);    //! write n data to buffer 
uint16_t fifoPopBuf(uint8_t* out_buf, uint16_t n);  //! read n data from buffer 
uint16_t fifoGetBufDataCount(void);                 //! get unread data count in buffer 
void fifoFlushBuf(void);                            //! flush buffer 
void fifoLookBuf(uint8_t* out_buf);                 //! look data in buffer, but not read out 


#endif



