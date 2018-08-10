


#include "main.h"
#include "esp8266helper.h"
#include "esp8266.h"
#include "debug.h"
#include "eeprom.h"
#include "data_struct.h"
#include "task_wifi.h"

uint8_t test[10] = "1234567890";

typedef enum 
{
	WIFI_MODE_IDLE = 0,
    WIFI_MODE_JUDGE,    //! 判断是否已连接WIFI
	WIFI_MODE_INIT,
    WIFI_MODE_STA,      //! 设置为STA模式
    WIFI_MODE_CIPMUX,   //! 关闭多路链接
    WIFI_MODE_CWJAP,    //! 连接到指定WIFI
    WIFI_MODE_IP,       //! 检查IP
    WIFI_MODE_SERVER,   //! 连接到指定服务器
    WIFI_MODE_CIPMODE,  //! 设置透传模式
    WIFI_MODE_CIPSEND,  //! 启用透传模式
	WIFI_MODE_RUNNING,
    WIFI_MODE_RST,
} wifi_mode_t; 


void osTaskWifi(void *pParameters)
{
    static wifi_mode_t state = WIFI_MODE_IDLE;
    static volatile TickType_t currentTime;
    
    vTaskDelay(100);
    
	while(1) 
	{   //wdgFeed();          
        switch(state) {
            case WIFI_MODE_IDLE:
                debug("wifi mode: idle \n");
            
                wifiSoftReset();
                state = WIFI_MODE_INIT;               
                break;

            case WIFI_MODE_INIT:
                debug("wifi mode: init \n");
            
                wifiInit();
                state = WIFI_MODE_JUDGE;
                break;
            
            case WIFI_MODE_JUDGE:
                debug("wifi mode: judge \n");
            
                if (wifiSmartlink() == true) {
                    state = WIFI_MODE_STA;
                    
                } else {
                    state = WIFI_MODE_CWJAP;
                }
                break;

            case WIFI_MODE_CWJAP:
                debug("wifi mode: cwjap \n");
            
                if (wifiSendCmd("AT+CWJAP=\"TP-LINK_6B0F\",\"hxgk8008\"", "WIFI GOT IP", 100) == true) {  //! 连接到指定WIFI
                    state = WIFI_MODE_STA;
                    
                } else {
                    state = WIFI_MODE_CWJAP;
                }            
                break;
                            
            case WIFI_MODE_STA:
                debug("wifi mode: sta \n");
            
                if (wifiSendCmd("AT+CWMODE=1", "OK", 50) == true) {  //! 设置8266为STA模式
                    state = WIFI_MODE_CIPMUX;
                    
                } else {
                    state = WIFI_MODE_STA;
                }               
                break;

            case WIFI_MODE_CIPMUX:
                debug("wifi mode: cipmux \n");
            
                if (wifiSendCmd("AT+CIPMUX=0", "OK", 50) == true) {  //! 关闭多路链接
                    state = WIFI_MODE_IP;
                    
                } else {
                    state = WIFI_MODE_CIPMUX;
                }                           
                break;

            case WIFI_MODE_IP:
                debug("wifi mode: ip check \n");
            
                wifiGetIp();
                state = WIFI_MODE_SERVER;
                break;
            
            case WIFI_MODE_SERVER:
                debug("wifi mode: server \n");  
            
                if (wifiSendCmd("AT+CIPSTART=\"TCP\",\"192.168.0.100\",\"5000\"", "OK", 50) == true) {  //! 连接到服务器
                    state = WIFI_MODE_CIPMODE;
                    
                } else {
                    state = WIFI_MODE_SERVER;
                }                
                break;

            case WIFI_MODE_CIPMODE:
                debug("wifi mode: cipmode \n");
                if (wifiSendCmd("AT+CIPMODE=1", "OK", 50) == true) {  //! 开启透传
                    state = WIFI_MODE_CIPSEND;
                    
                } else {
                    state = WIFI_MODE_CIPMODE;
                }            
                break;

            case WIFI_MODE_CIPSEND:
                debug("wifi mode: cipsend \n");

                if (wifiSendCmd("AT+CIPSEND", ">", 50) == true) {  //! 开启传输模式
                    state = WIFI_MODE_RUNNING; 
                    
                } else {
                    state = WIFI_MODE_CIPSEND; 
                }           
                break;
            
            case WIFI_MODE_RUNNING:
                debug("wifi mode: running \n");
            
                if (xTaskGetTickCount() - currentTime >= 500) { 
                currentTime = xTaskGetTickCount();
                wifiSendDataPassthrough(test, sizeof(test));
                }
                break;
            
            case WIFI_MODE_RST:
                debug("wifi mode: reset \n");
            
                state = WIFI_MODE_IDLE;
                break;
        }
        
        vTaskDelay(1);
	}
	
}

