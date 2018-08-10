
#include "eeprom.h"
#include "message.h"
#include "board.h"
#include "main.h"
#include "MQTTHelper.h"
#include "data_struct.h"

//#define  DEBUG_SERIAL

static char TOPIC_GPRS[21] = {"TP_GPRS_"};

bool msg_serial_parser(mqtt_message_t *rxmsg, uint8_t c)
{
	bool rc = false;
	static mqtt_message_t rxmsg_t;	
	static uint16_t bufferIndex;
	static msg_serial_parse_state_t status;
    

	switch (status)
	{
	case SERIAL_PARSE_STATE_UNINIT:
	case SERIAL_PARSE_STATE_IDLE: 
		if (c == STX) { 
			rxmsg_t.head.stx = c;
			status = SERIAL_PARSE_STATE_GOT_STX; 
            
		} else {
            status = SERIAL_PARSE_STATE_IDLE;    
        }            
		break;

    case SERIAL_PARSE_STATE_GOT_STX:
		if (c == SEQ) { 
			rxmsg_t.head.seq = c;
			status = SERIAL_PARSE_STATE_GOT_SEQ; 
            
		} else {
            status = SERIAL_PARSE_STATE_IDLE; 
        }            
		break;
        
	case SERIAL_PARSE_STATE_GOT_SEQ: 
        rxmsg_t.head.id = c;
        status = SERIAL_PARSE_STATE_GOT_ID;
		break;

	case SERIAL_PARSE_STATE_GOT_ID:    
        rxmsg_t.head.len = c;
        status = SERIAL_PARSE_STATE_GOT_LEN;
		break;
	
	case SERIAL_PARSE_STATE_GOT_LEN:  
		rxmsg_t.payload[bufferIndex++] = c;
		if (bufferIndex == ((rxmsg_t.head.len)-6)) {
			status = SERIAL_PARSE_STATE_GOT_PAYLOAD;
		}
		break;

	case SERIAL_PARSE_STATE_GOT_PAYLOAD: 
		rxmsg_t.crc_l = c;                
		status = SERIAL_PARSE_STATE_GOT_CRC1;
		break;

	case SERIAL_PARSE_STATE_GOT_CRC1: 
        rxmsg_t.crc_h = c;      
        if (crc16((uint8_t*)&rxmsg_t.head.stx, rxmsg_t.head.len - 2) == ((uint16_t)rxmsg_t.crc_h << 8) + rxmsg_t.crc_l) {
            memcpy(&rxmsg->head.stx, &rxmsg_t.head.stx, rxmsg_t.head.len - 2);
            memcpy(&rxmsg->crc_l, &rxmsg_t.crc_l, 2);
            rc = true;
        }
        
		bufferIndex = 0;                                   
		status = SERIAL_PARSE_STATE_UNINIT;		
		break;
	
	default:
		bufferIndex = 0;
		status = SERIAL_PARSE_STATE_UNINIT;

	}
	
	return rc;
}



void handleSerialMessage(const mqtt_message_t* msg)
{

    switch(msg->head.id)
    {
        case MSG_MQTT_ID_TIME:
            handle_msg_serial_set_time(msg);
            break;
            
        case MSG_MQTT_ID_REBOOT:
            handle_msg_serial_reboot(msg);
            break;	

        case MSG_MQTT_ID_IO_CTRL:
            handle_msg_serial_io_ctrl(msg);
            break;
        
        case MSG_MQTT_ID_OPERATE:
            handle_msg_serial_set_operate(msg);
            break;
        
        case MSG_MQTT_ID_MQTT:
            handle_msg_serial_set_mqtt(msg);
            break;

        case MSG_MQTT_ID_LNET:
            handle_msg_serial_set_lnet(msg);
            break;
        
        case MSG_MQTT_ID_IAP:
            handle_msg_serial_set_iap(msg);
            break;	
        
        case MSG_MQTT_ID_LOGIN:
            handle_msg_serial_set_login(msg);
            break;
        
        case MSG_MQTT_ID_CHZ:
            handle_msg_serial_set_chz(msg);
            break;  

        case MSG_MQTT_ID_SEN:
            handle_msg_serial_set_sen(msg);
            break; 

        case MSG_MQTT_ID_PMU:
            handle_msg_serial_set_pmu(msg);
            break;
    }
        
}



#if 0
void handleMqttMessage(const mqtt_message_t* msg)
{

    switch(msg->head.id)
    {            
        case MSG_MQTT_ID_REBOOT:
            handle_msg_mqtt_reboot(msg);
            break;	

        case MSG_MQTT_ID_IO_CTRL:
            handle_msg_mqtt_io_ctrl(msg);
            break;
        
        case MSG_MQTT_ID_OPERATE:
            handle_msg_mqtt_set_operate(msg);
            break;
        
        case MSG_MQTT_ID_MQTT:
            handle_msg_mqtt_set_mqtt(msg);
            break;

        case MSG_MQTT_ID_LNET:
            handle_msg_mqtt_set_lnet(msg);
            break;
        
        case MSG_MQTT_ID_IAP:
            handle_msg_mqtt_set_iap(msg);
            break;	
        
        case MSG_MQTT_ID_LOGIN:
            handle_msg_mqtt_set_login(msg);
            break;
        
        case MSG_MQTT_ID_CHZ:
            handle_msg_mqtt_set_chz(msg);
            break;  

        case MSG_MQTT_ID_SEN:
            handle_msg_mqtt_set_sen(msg);
            break; 

        case MSG_MQTT_ID_PMU:
            handle_msg_mqtt_set_pmu(msg);
            break;
        
        case MSG_MQTT_ID_ALARM:
            handle_msg_mqtt_alarm(msg);
            break;
        
    }
        
}


void handle_msg_mqtt_alarm(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_ALARM_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_ALARM_LEN-2);
    memcpy(buffer+MSG_ID_ALARM_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_ALARM_LEN);
}


void handle_msg_mqtt_reboot(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_REBOOT_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_REBOOT_LEN-2);
    memcpy(buffer+MSG_ID_REBOOT_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_REBOOT_LEN);
}


void handle_msg_mqtt_io_ctrl(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_IO_CTRL_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_IO_CTRL_LEN-2);
    memcpy(buffer+MSG_ID_IO_CTRL_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_IO_CTRL_LEN);
}

void handle_msg_mqtt_set_operate(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_OPERATE_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_OPERATE_LEN-2);
    memcpy(buffer+MSG_ID_OPERATE_LEN-2, &msg->crc_l, 2);

    usart1SendBuffer(buffer, MSG_ID_OPERATE_LEN);    
}


void handle_msg_mqtt_set_iap(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_IAP_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_IAP_LEN-2);
    memcpy(buffer+MSG_ID_IAP_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_IAP_LEN); 
}


void handle_msg_mqtt_set_lnet(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_LNET_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_LNET_LEN-2);
    memcpy(buffer+MSG_ID_LNET_LEN-2, &msg->crc_l, 2);
}


void handle_msg_mqtt_set_mqtt(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_MQTT_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_MQTT_LEN-2);
    memcpy(buffer+MSG_ID_MQTT_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_MQTT_LEN);
}



void handle_msg_mqtt_set_login(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_LOGIN_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_LOGIN_LEN-2);
    memcpy(buffer+MSG_ID_LOGIN_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_LOGIN_LEN);
}


void handle_msg_mqtt_set_chz(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_CHZ_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_CHZ_LEN-2);
    memcpy(buffer+MSG_ID_CHZ_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_CHZ_LEN);
}


void handle_msg_mqtt_set_sen(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_SEN_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_SEN_LEN-2);
    memcpy(buffer+MSG_ID_SEN_LEN-2, &msg->crc_l, 2);
    
    usart1SendBuffer(buffer, MSG_ID_SEN_LEN);
}


void handle_msg_mqtt_set_pmu(const mqtt_message_t *msg)
{
    static unsigned char buffer[MSG_ID_PMU_LEN];
	memcpy(buffer, &msg->head.stx, MSG_ID_PMU_LEN-2);
    memcpy(buffer+MSG_ID_PMU_LEN-2, &msg->crc_l, 2); 

    usart1SendBuffer(buffer, MSG_ID_PMU_LEN);    
}


#endif





void handle_msg_serial_set_time(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);

    static char buffer[MSG_ID_TIME_LEN];
    memcpy(buffer, &msg->head.stx, MSG_ID_TIME_LEN);
    memcpy(buffer+MSG_ID_TIME_LEN-2, &msg->crc_l, 2);

    mqtt_publish_buffer(TOPIC_TIME, buffer, MSG_ID_TIME_LEN);    
}


void handle_msg_serial_reboot(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    
    static char buffer[MSG_ID_REBOOT_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6); 
    memcpy(buffer, &msg->head.stx, MSG_ID_REBOOT_LEN);
    memcpy(buffer+MSG_ID_REBOOT_LEN-2, &msg->crc_l, 2); 

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_REBOOT_LEN);      
}


void handle_msg_serial_io_ctrl(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_IO_CTRL_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);    
    memcpy(buffer, &msg->head.stx, MSG_ID_IO_CTRL_LEN);
    memcpy(buffer+MSG_ID_IO_CTRL_LEN-2, &msg->crc_l, 2);

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_IO_CTRL_LEN);    
}


void handle_msg_serial_set_operate(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_OPERATE_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_OPERATE_LEN);
    memcpy(buffer+MSG_ID_OPERATE_LEN-2, &msg->crc_l, 2);

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_OPERATE_LEN);     
}


void handle_msg_serial_set_login(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_LOGIN_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6); 
    memcpy(buffer, &msg->head.stx, MSG_ID_LOGIN_LEN);
    memcpy(buffer+MSG_ID_LOGIN_LEN-2, &msg->crc_l, 2); 

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_LOGIN_LEN);     
}


void handle_msg_serial_set_lnet(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
        
    static char buffer[MSG_ID_LNET_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_LNET_LEN);
    memcpy(buffer+MSG_ID_LNET_LEN-2, &msg->crc_l, 2); 

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_LNET_LEN);    
}


void handle_msg_serial_set_mqtt(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_MQTT_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_MQTT_LEN);
    memcpy(buffer+MSG_ID_MQTT_LEN-2, &msg->crc_l, 2);

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_MQTT_LEN);    
}


void handle_msg_serial_set_gprs(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_GPRS_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_GPRS_LEN);
    memcpy(buffer+MSG_ID_GPRS_LEN-2, &msg->crc_l, 2); 

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_GPRS_LEN);     
}


void handle_msg_serial_set_chz(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_CHZ_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_CHZ_LEN);
    memcpy(buffer+MSG_ID_CHZ_LEN-2, &msg->crc_l, 2); 

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_CHZ_LEN);    
}


void handle_msg_serial_set_sen(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_SEN_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_SEN_LEN);
    memcpy(buffer+MSG_ID_SEN_LEN-2, &msg->crc_l, 2); 

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_SEN_LEN);     
}


void handle_msg_serial_set_pmu(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_PMU_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_PMU_LEN);
    memcpy(buffer+MSG_ID_PMU_LEN-2, &msg->crc_l, 2);
    
    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_PMU_LEN);
}


void handle_msg_serial_set_iap(const mqtt_message_t *msg)
{
    printf("id %02X \n", msg->head.id);
    
    static char buffer[MSG_ID_IAP_LEN];
    convertHexToStr((uint8_t*)&TOPIC_GPRS[8], (uint8_t*)&msg->payload[0], 6);
    memcpy(buffer, &msg->head.stx, MSG_ID_IAP_LEN);
    memcpy(buffer+MSG_ID_IAP_LEN-2, &msg->crc_l, 2);

    mqtt_publish_buffer(TOPIC_GPRS, buffer, MSG_ID_IAP_LEN);    
}


