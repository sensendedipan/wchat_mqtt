
#ifndef _ESP8266_HELPER_H
#define _ESP8266_HELPER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include "board.h"
#include "utility.h"
#include "debug.h"
#include "data_struct.h"


#define WIFI_RX_BUF_MAX     512
#define wifiSendChar(a)		usart2SendChar(a)
#define wifiSendStr(a)		usart2SendString(a)
#define wifiSendBuf(a,b)    usart2SendBuffer(a,b)

extern uint8_t  wifiRxBuf[WIFI_RX_BUF_MAX];

bool wifiFindStr(char *a);
void wifiClearWifiBuf(void);
bool wifiSendCmd(char *cmd, char *ack, uint8_t holdTime);
bool wifiCmdCheck(char *ack, uint8_t holdTime);
uint8_t convertStrToInt(uint8_t *p, uint8_t len);
void swap(char *ch1, char *ch2);
void convertIntToStr(int n, char str[]);
void convertHexToStr(uint8_t dest[], uint8_t src[], uint8_t len);
bool wifiSendData(uint8_t *buf, uint8_t len);
void wifiSendDataPassthrough(uint8_t *buf, uint8_t len);
int  wifiGetDataPassthrough(uint8_t *buf, uint8_t len);




#endif