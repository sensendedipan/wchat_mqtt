#ifndef __TCP_H
#define __TCP_H


#include <stm32f4xx.h>
#include <stdio.h>
#include "types.h"



#define TCP_SEND_MS	    1000 
#define TCP_RX_BUF_SIZE 256
#define	TCP_TX_BUF_SIZE	1000


void runTCPClient(void);


#endif 

