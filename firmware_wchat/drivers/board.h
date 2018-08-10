

#ifndef _BOARD_H
#define _BOARD_H



#include <stm32f4xx.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"


#include "eeprom.h"
#include "fifo.h"
#include "fiforx.h"
#include "crc16.h"
#include "flash.h"

#include "wdg_driver.h"
#include "led_driver.h"
#include "adc_driver.h"
#include "inout_driver.h"
#include "i2c_driver.h"
#include "i2c2_driver.h"
#include "spi1_driver.h"
#include "spi2_driver.h"
#include "at24cx_driver.h"
#include "w25qx_driver.h"
#include "usart1_driver.h"
#include "usart2_driver.h"
#include "usart3_driver.h"
#include "usart4_driver.h"
#include "usart6_driver.h"
#include "timer4_driver.h"
#include "timer2_driver.h"
#include "timer3_driver.h"
#include "timer5_driver.h"

#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"
#include "netbios.h"
#include "ntp.h"
#include "tftp.h"



void boardInit(void);


#endif


