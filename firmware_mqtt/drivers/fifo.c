

#include "fifo.h"
#include "board.h"



static uint8_t buf_flags;
static uint16_t  in_index, out_index;
static uint8_t buffer[FIFO_BUF_SIZE];




/**
  * @brief  Write n data to buffer
  * @retval None
  */
uint16_t fifoPushBuf(uint8_t* data, uint16_t n)
{
	uint16_t i = 0;
	uint16_t data_count = 0;

	while (n--) {
		if (buf_flags & OVREFLOW_FLAG1) {
			return 0;
            
		} else {
			buffer[in_index++] = data[i++];
			in_index &= FIFO_BUF_MASK;
			buf_flags &= ~EMPTY_FLAG1;
			data_count++;
			if (in_index == out_index) {
				buf_flags |= OVREFLOW_FLAG1;
			}
		}
	}
	return data_count;
}



/**
  * @brief  Flush buffer
  * @retval None
  */
void fifoFlushBuf(void)
{
	out_index = in_index;
	buf_flags |= EMPTY_FLAG1;
}



/**
  * @brief  Read n data from buffer
  * @retval None
  */
uint16_t fifoPopBuf(uint8_t* out_buf, uint16_t n)
{
	uint16_t i = 0;
	uint16_t data_count = 0;

	while (n--) {
		if (out_index == in_index) {
			if (!(buf_flags & OVREFLOW_FLAG1)) {
				buf_flags |= EMPTY_FLAG1;
				return 0;
			}
		}
		out_buf[i++] = buffer[out_index++];
		out_index &= FIFO_BUF_MASK;
		data_count++;
		buf_flags &= ~OVREFLOW_FLAG1;
	}
	return data_count;
}



/**
  * @brief  Get unread data count in buffer
  * @retval None
  */
uint16_t fifoGetBufDataCount(void)
{
	if (out_index == in_index) {
		if (!(buf_flags & OVREFLOW_FLAG1)) {
			buf_flags |= EMPTY_FLAG1;
			return 0;
			
		} else {
			return FIFO_BUF_SIZE;
		}
	}
	if (in_index > out_index) {
		return in_index - out_index;
		
	} else {
		return FIFO_BUF_SIZE - out_index + in_index;
	}
}



/**
  * @brief  Look data in buffer, but not read out
  * @retval None
  */
void fifoLookBuf(uint8_t* out_buf)
{
	uint16_t i = 0;
	while (i < FIFO_BUF_SIZE) {
		out_buf[i] = buffer[i];
		i++;
	}
}




