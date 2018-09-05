
#ifndef _MQTT_HELPER_H
#define _MQTT_HELPER_H

#include "board.h"

#include "MQTTFormat.h"
#include "MQTTPacket.h"
#include "MQTTConnect.h"

#define MQTT_QOS0           0
#define MQTT_QOS1           1
#define MQTT_QOS2           2

#define TOPIC_TIME          "TP_TIME"       //! sub msg: to update time
#define TOPIC_STAE          "TP_GPRS"
#define TOPIC_ALARM         "TP_ALARM"
#define TOPIC_ACK           "TP_ACK"

#define MQTT_KEEP_ALIVE     60000 

bool transport_open(void);
bool transport_close(void);


void mqtt_get_message(bool* state);
bool mqtt_connect(char* client_id, char* user, char* password);
bool mqtt_subscrib(char* pTopic, int qos);
bool mqtt_publish_buffer(char* pTopic, char *pMsg, uint16_t msg_len);

bool mqtt_pingreq(void);
bool mqtt_disconnect(void);

#endif
