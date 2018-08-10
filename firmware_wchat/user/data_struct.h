

#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H

#include <stm32f4xx.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>



//! IP 获取方式
#define IP_STATIC       0X00	
#define IP_DYNAMIC      0X01

//! 是否使用域名	
#define DNS_DISABLE     0X00
#define DNS_ENABLE      0X01

//! 重合闸是否启用	
#define CHZ_DISABLE     0X00
#define CHZ_ENABLE      0X01


//! 220V交流输入
#define AC_IN_OK        0X01   
#define AC_IN_NO        0X00
#define AC_IN_UNVALID   0XFF



enum 
{
    KM_CHAN_KM1 = 0,  
    KM_CHAN_KM2,
    KM_CHAN_KM3,
    KM_CHAN_KM4,
    KM_CHAN_NUM,
};


enum 
{
    SW_CHAN_SW1 = 0,  
    SW_CHAN_SW2,
    SW_CHAN_SW3,
    SW_CHAN_SW4,
    SW_CHAN_NUM,
};


typedef enum 
{
	MQTT_MODE_IDLE = 0,
	MQTT_MODE_INIT,
    MQTT_MODE_PHY,
    MQTT_MODE_DHCP,
    MQTT_MODE_DNS,    
	MQTT_MODE_CONNECT,
	MQTT_MODE_RUNNING,
	MQTT_MODE_ETHREBOOT	
} mqtt_mode_t; 


typedef enum 
{ 
	STATE_PHY = 0,
    STATE_DHCP,
    STATE_DNS,
    STATE_CONNECT,
    STATE_RUNNING

} run_state_t;


//////////////////////// eeprom 相关参数定义 ////////////////////////////////////////////////



typedef __packed struct __eeprom_lnet_t	                    
{  
    uint8_t  start;  //! 0X55 means this eeprom area has been inited before and it is valid
    uint8_t  ip[4];
    uint8_t  sub[4];
    uint8_t  gay[4];
    uint8_t  dns[4];
    uint16_t port;
    uint32_t baud;
    uint8_t  dhcp;   //! 0: static ip  1: dynamic ip 
    uint16_t checksum;
} eeprom_lnet_t;




typedef __packed struct __eeprom_mqtt_t	                    
{  
    uint8_t  start;  //! 0X55 means this eeprom area has been inited before and it is valid
    uint8_t  ip[4];    
    uint16_t port;
    uint8_t  dns; //! 0: ip  1: dns 
    uint8_t  url[50];
    uint16_t checksum;
} eeprom_mqtt_t;



typedef __packed struct __eeprom_mlgi_t	                    
{  
    uint8_t  start;  //! 0X55 means this eeprom area has been inited before and it is valid
    uint8_t  usr[6];
    uint8_t  psd[6]; 
    uint16_t checksum;
} eeprom_mlgi_t;



typedef __packed struct __eeprom_lgi_t	                    
{
    uint8_t  start;     //! 0X55 means this eeprom area has been inited before and it is valid
    uint8_t  oldusr[6]; //! !0 valid
    uint8_t  oldpsd[6]; //! !0 valid
    uint8_t  newusr[6]; //! !0 valid
    uint8_t  newpsd[6]; //! !0 valid
    uint8_t  valpsd[6]; //! !0 valid
    uint16_t checksum;
} eeprom_lgi_t;


#endif
