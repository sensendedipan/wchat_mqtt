


#ifndef _MESSAGES_H
#define _MESSAGES_H

#include "stm32f4xx.h" 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>                                    
#include <cstdlib>
#include "eeprom.h"
#include "inout_driver.h"
#include "w5500_conf.h"
#include "data_struct.h"

#define STX         0XFE
#define SEQ         0X01
#define MSG_LEN_MAX 245     //! can not be outside of 253

#define MSG_MQTT_ID_HEARTBEAT   0X00    //! client  -> server    trigger
#define MSG_MQTT_ID_STATE       0X01    //! client  -> server    periodicity
#define MSG_MQTT_ID_TIME        0X02    //! server  -> client    periodicity     
#define MSG_MQTT_ID_REBOOT      0X03    //! server <-> client    interaction
#define MSG_MQTT_ID_IO_CTRL     0X04    //! server <-> client    interaction
#define MSG_MQTT_ID_OPERATE     0X05    //! server <-> client    interaction
#define MSG_MQTT_ID_LNET        0X06    //! server <-> client    interaction
#define MSG_MQTT_ID_MQTT        0X07    //! server <-> client    interaction
#define MSG_MQTT_ID_GPRS        0X08    //! server <-> client    interaction
#define MSG_MQTT_ID_LOGIN       0X09    //! server <-> client    interaction
#define MSG_MQTT_ID_IAP         0X0A    //! server <-> client    interaction
#define MSG_MQTT_ID_CHZ         0X0B    //! server <-> client    interaction
#define MSG_MQTT_ID_SEN         0X0C    //! server <-> client    interaction
#define MSG_MQTT_ID_PMU         0X0D    //! server <-> client    interaction
#define MSG_MQTT_ID_ALARM       0X0E    //! client  -> server    trigger
#define MSG_MQTT_ID_PARAM       0X0F    //! client  -> server

#define MSG_ID_HEARTBEAT_LEN    0X1A
#define MSG_ID_STATE_LEN        0X8B
#define MSG_ID_TIME_LEN         0X13
#define MSG_ID_REBOOT_LEN       0X16
#define MSG_ID_IO_CTRL_LEN      0X16
#define MSG_ID_OPERATE_LEN      0X22
#define MSG_ID_LNET_LEN         0X27
#define MSG_ID_MQTT_LEN         0X5A
#define MSG_ID_GPRS_LEN         0X4D
#define MSG_ID_LOGIN_LEN        0X33
#define MSG_ID_IAP_LEN          0X37
#define MSG_ID_CHZ_LEN          0X25
#define MSG_ID_SEN_LEN          0X23
#define MSG_ID_PMU_LEN          0X24
#define MSG_ID_ALARM_LEN        0X17


typedef enum msg_serial_parse_state_t
{
    SERIAL_PARSE_STATE_UNINIT = 0,
    SERIAL_PARSE_STATE_IDLE,
    SERIAL_PARSE_STATE_GOT_STX,
    SERIAL_PARSE_STATE_GOT_SEQ,
    SERIAL_PARSE_STATE_GOT_ID,
	SERIAL_PARSE_STATE_GOT_LEN,    
    SERIAL_PARSE_STATE_GOT_PAYLOAD,
    SERIAL_PARSE_STATE_GOT_CRC1,
	SERIAL_PARSE_STATE_GOT_CRC2,
	SERIAL_PARSE_STATE_GOT_OK
} msg_serial_parse_state_t; 



typedef __packed struct __mqtt_msg_head_t 
{	 	
	uint8_t     stx;   //! packet start sign, 0XFE
	uint8_t     seq;   //! msg sequence,      0X00 
	uint8_t	    id;    //! 0X01 0X02 .....
    uint8_t     len;   //! whole msg len
} mqtt_msg_head_t;



typedef __packed struct __mqtt_message_t 
{	 	
    mqtt_msg_head_t head;  
	uint8_t         payload[MSG_LEN_MAX];
    uint8_t         crc_l;
    uint8_t         crc_h;
} mqtt_message_t;



bool msg_serial_parser(mqtt_message_t *rxmsg, uint8_t c);
void msg_mqtt_parser(const uint8_t* buf, uint16_t len);
void handleSerialMessage(const mqtt_message_t* msg);

void handle_msg_mqtt_alarm(const mqtt_message_t *msg);
void handle_msg_mqtt_set_time(const mqtt_message_t *msg);
void handle_msg_mqtt_reboot(const mqtt_message_t *msg);
void handle_msg_mqtt_io_ctrl(const mqtt_message_t *msg);
void handle_msg_mqtt_set_operate(const mqtt_message_t *msg);
void handle_msg_mqtt_set_login(const mqtt_message_t *msg);
void handle_msg_mqtt_set_lnet(const mqtt_message_t *msg);
void handle_msg_mqtt_set_mqtt(const mqtt_message_t *msg);
void handle_msg_mqtt_set_gprs(const mqtt_message_t *msg);
void handle_msg_mqtt_set_chz(const mqtt_message_t *msg);
void handle_msg_mqtt_set_sen(const mqtt_message_t *msg);
void handle_msg_mqtt_set_pmu(const mqtt_message_t *msg);
void handle_msg_mqtt_set_iap(const mqtt_message_t *msg);

void handle_msg_serial_set_time(const mqtt_message_t *msg);
void handle_msg_serial_reboot(const mqtt_message_t *msg);
void handle_msg_serial_io_ctrl(const mqtt_message_t *msg);
void handle_msg_serial_set_operate(const mqtt_message_t *msg);
void handle_msg_serial_set_login(const mqtt_message_t *msg);
void handle_msg_serial_set_lnet(const mqtt_message_t *msg);
void handle_msg_serial_set_mqtt(const mqtt_message_t *msg);
void handle_msg_serial_set_gprs(const mqtt_message_t *msg);
void handle_msg_serial_set_chz(const mqtt_message_t *msg);
void handle_msg_serial_set_sen(const mqtt_message_t *msg);
void handle_msg_serial_set_pmu(const mqtt_message_t *msg);
void handle_msg_serial_set_iap(const mqtt_message_t *msg);


#endif


