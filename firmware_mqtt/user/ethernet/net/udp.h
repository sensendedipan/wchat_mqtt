#ifndef __UDP_H
#define __UDP_H

#include "stm32f4xx.h"
#include "Types.h"

#define UDP_SEND_MS     1000
#define UDP_TX_BUF_SIZE 128
#define UDP_RX_BUF_SIZE 128


void runUDPClient(void);

#endif 


