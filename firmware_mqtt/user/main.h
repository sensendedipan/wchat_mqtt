

#ifndef _MAIN_H
#define _MAIN_H

#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "eeprom.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
 
#include "socket.h"
#include "w5500.h"
#include "utility.h"
#include "w5500_conf.h"
#include "dhcp.h"
#include "web_server.h"
#include "webutil.h"








extern bool gFw5500LinkState;


extern eeprom_lgi_t   gEepromLgi;

extern eeprom_mqtt_t  gEepromMqtt;
extern eeprom_lnet_t  gEepromLnet;
extern eeprom_mlgi_t  gEepromMlgi;

void osTaskDog(void *pParameters);
void osTaskMqt(void *pParameters);
void osTaskWeb(void *pParameters);
void osTaskWifi(void *pParameters);




#endif
