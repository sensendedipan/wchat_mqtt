


#ifndef _ESP8266_H
#define _ESP8266_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include "board.h"
#include "utility.h"


void wifiInit(void);
void wifiGetIp(void);
void wifiSetATE0(void);
void wifiSoftReset(void);
bool wifiSmartlink(void);
void wifiConnectToServer(void);
void wifiExitTransfer(void);
void wifiSetServerIPAddr(bool dns_enable, uint8_t url[50], uint8_t ip[4], uint16_t port, char addr[80]);


#endif