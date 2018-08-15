
#include "MQTTHelper.h"
#include "w5500_conf.h"
#include "debug.h"



//#define MQTT_DEBUG

#define MQTT_RX_BUF_SIZE 1024
#define	MQTT_TX_BUF_SIZE 1024

       

#define	MQTT_DATA_BUF_SIZE 8192

static uint8_t  mqtt_broker_ip[4] = {0, 0, 0, 0};
static uint16_t mqtt_broker_port = 0;

static uint8_t mqttTXBuf[MQTT_TX_BUF_SIZE];
static uint8_t mqttRXBuf[MQTT_RX_BUF_SIZE];
static uint8_t mqttDataBuf[MQTT_DATA_BUF_SIZE];

extern void handleTopics(char* recTopic, char* pMessage, uint16_t len);

static bool transport_sendPacketBuffer(unsigned char* buf, int buflen)
{  
    uint16_t i = 0;
    while ((getSn_SR(SOCK_TCPS) != SOCK_ESTABLISHED)&&(getSn_SR(SOCK_TCPS) != SOCK_CLOSE_WAIT)) {  //! must do this added by apple
        if (i++ > 3000) {
#ifdef MQTT_DEBUG             
            debug("send data: i: %d error socket state:%x \n", i, getSn_SR(SOCK_TCPS));
#endif
            return false;
        }
    }
    
    if (send(SOCK_TCPS, buf, buflen) == buflen) {
        return true;
        
    } else {
        return false;
    }        
}




static int transport_getdata(unsigned char* buf, int count)
{  
    uint16_t i = 0; 
    while (getSn_SR(SOCK_TCPS) != SOCK_ESTABLISHED) {
        if (i++ > 3000) {
#ifdef MQTT_DEBUG             
            debug("get data: error socket state:%x \n", getSn_SR(SOCK_TCPS));
#endif                   
            break;
        }
    }
    
    int32_t len = getSn_RX_RSR(SOCK_TCPS);
    if (len > count) {
        return recv(SOCK_TCPS, buf, count);

    } else if (len > 0) {
        return recv(SOCK_TCPS, buf, len);
    
    } else {       
        return 0;
    }   
}



bool transport_open(void)
{ 
    int32_t ret;
    static uint16_t local_port = 5000;
    static const uint32_t timeout = 5000;
    memcpy(mqtt_broker_ip, gEepromMqtt.ip, 4);  
    mqtt_broker_port = gEepromMqtt.port;
    ret = socket(SOCK_TCPS, Sn_MR_TCP, local_port++, Sn_MR_ND);
    if (ret != 1) {
#ifdef MQTT_DEBUG         
        printf("transport_open: socket open failed %d !", ret);
#endif         
        return false;
    }
    
    ret = connect(SOCK_TCPS, mqtt_broker_ip, mqtt_broker_port);
    if (ret != 1) {
#ifdef MQTT_DEBUG         
        debug("transport_open: socket connect failed %d !", ret);
#endif         
        return false;
        
    } 	

    for (uint32_t i = 0; i < timeout; i++) {
        if (getSn_SR(SOCK_TCPS) == SOCK_ESTABLISHED) {
            return true;
        
        } else if (i == timeout - 1) {
            return false;                
        
        } else {
            vTaskDelay(2);
        }            
    }    
}



bool transport_close(void)
{   
    disconnect(SOCK_TCPS);
    close(SOCK_TCPS); 
    while(getSn_SR(SOCK_TCPS) != SOCK_CLOSED);  //! block
    return true;   
}               



/**
*@name     mqtt_connect()
*@brief	   
*@param		
*@return   connect ok:true  connect failed: false
*/
bool mqtt_connect(char* client_id, char* user, char* password)
{
    int rc = 0;
    int len = 0;
    static char usr[7] = {0};
    static char psd[7] = {0};
    int buflen = sizeof(mqttTXBuf);
    memset(mqttTXBuf, 0, buflen);
    static uint8_t connect_req_times = 0;
    const static uint8_t connect_req_max = 5;
    memcpy(usr, user, 6); 
    memcpy(psd, password, 6);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    data.clientID.cstring = client_id;
    data.keepAliveInterval = MQTT_KEEP_ALIVE;
    data.cleansession = 1;
    data.username.cstring = usr;
    data.password.cstring = psd;

    if (transport_open() == false) {
        connect_req_times = 0;
        return false;
    } 
    
    while(connect_req_times++ < connect_req_max) { 

        len = MQTTSerialize_connect(mqttTXBuf, buflen, &data);        
        bool result = transport_sendPacketBuffer(mqttTXBuf, len); 
        
        if (result == false) {
            vTaskDelay(1);
            
        } else {
            connect_req_times = 0;
            break;
        }
        
        if (connect_req_times >= (connect_req_max-1)) {
            connect_req_times = 0;          
            return false;
        }
    }

    //! wait for connack
    static uint8_t wait_for_ack_times = 0;
    const static uint8_t wait_for_ack_max = 5; 
    while(wait_for_ack_times++ < wait_for_ack_max) {
        vTaskDelay(250*(wait_for_ack_times+1));
        if (MQTTPacket_read(mqttTXBuf, buflen, transport_getdata) == CONNACK) {
            unsigned char sessionPresent, connack_rc;
            if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, mqttTXBuf, buflen) != 1 || connack_rc != 0) {
#ifdef MQTT_DEBUG            
                debug("mqtt_connect: wait for connack ack %d times and error code:%X \n", wait_for_ack_times, connack_rc);
#endif                 
            } else {
                wait_for_ack_times = 0;
#ifdef MQTT_DEBUG            
            debug("mqtt_connect: succeed! \n");
#endif                
                return true;
            }
            
        } else {

        }

        if (wait_for_ack_times >= (wait_for_ack_max-1)) {
            wait_for_ack_times = 0;
#ifdef MQTT_DEBUG            
            debug("mqtt_connect: failed! \n");
#endif       
            mqtt_disconnect();
            return false;
        } 
      
    }

    wait_for_ack_times = 0;    
    return false;
}



/**
*@name     mqtt_publish_buffer()
*@brief	   
*@param		
*@return   publish ok:true  publish failed: false
*/
bool mqtt_publish_buffer(char* pTopic, char *pMsg, uint16_t msg_len)
{  
    int32_t len = 0;
    int msglen = msg_len;    
    int buflen = sizeof(mqttTXBuf);
    if (msglen > buflen) {
        return false;
    }
    memset(mqttTXBuf, 0, buflen);
    MQTTString topicString = MQTTString_initializer;
     
    const static unsigned char mqtt_dup = 0;        // first time to send
    const static int mqtt_qos = 0;                  // 1 is ok for our project
    const static unsigned char mqtt_retained = 0;   // just publish this msg to the client who is online right now.
    const static unsigned short mqtt_packetid = 0;   
    topicString.cstring = pTopic;

    len = MQTTSerialize_publish(mqttTXBuf, buflen, mqtt_dup, mqtt_qos, mqtt_retained, mqtt_packetid, topicString, (unsigned char*)pMsg, msglen);                 
    bool result = transport_sendPacketBuffer(mqttTXBuf, len);
    
    if (result == false) return false;     
}





/**
*@name     mqtt_subscrib()
*@brief	   
*@param	   qos: this gives the maximum Qos level at which the server can send application messages to the client
*@return   subscrib ok:true  subscrib failed: false
*/
bool mqtt_subscrib(char* pTopic, int qos)
{
    int rc = 0;
    int len = 0;
    int buflen = sizeof(mqttRXBuf);
    memset(mqttRXBuf, 0, buflen);
    unsigned char dup = 0;
    int packetid = 1;   
    static uint8_t sub_req_times = 0;
    const static uint8_t sub_req_max = 5;

    MQTTString topicString = MQTTString_initializer;
    topicString.cstring = pTopic;
          
    while(sub_req_times++ < sub_req_max) { 
        len = MQTTSerialize_subscribe(mqttRXBuf, buflen, dup, packetid, 1, &topicString, &qos);        
        bool result = transport_sendPacketBuffer(mqttRXBuf, len); 
        
        if (result == false) {
#ifdef MQTT_DEBUG            
            debug("mqtt_sub_req: send sub req failed %d times! \n", sub_req_times);
#endif  
        } else {
#ifdef MQTT_DEBUG            
            debug("mqtt_sub_req: send sub req succeed! \n");
#endif
            sub_req_times = 0;
            break;
        }
        
        if (sub_req_times >= (sub_req_max-1)) {
            sub_req_times = 0;
            return false;
        }   
    }    

    uint8_t wait_for_suback_times = 0;
    const static uint8_t wait_for_suback_max = 10;
    while(wait_for_suback_times++ < wait_for_suback_max) {
        //transport_sendPacketBuffer(mqttTXBuf, len);
        vTaskDelay(10*(wait_for_suback_times+1)); //! must do this and the more the better in www
        if (MQTTPacket_read(mqttRXBuf, buflen, transport_getdata) == SUBACK) {
            unsigned short submsgid;
            int subcount;
            int granted_qos; //! apple: allowed return codes: 0-maximus Qos0; 1-maximus Qos1; 2-maximus Qos2; 0x80(128): failure
            rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, mqttRXBuf, buflen);
            if ((granted_qos != qos)||(granted_qos == 0x80)) { //! failed!
#ifdef MQTT_DEBUG            
                debug("mqtt_sub_ack: wait for suback %d times, granted_qos code: %d \n", wait_for_suback_times, granted_qos);
#endif                 
            } else {
                wait_for_suback_times = 0;
#ifdef MQTT_DEBUG            
                debug("mqtt_subscribe: topic: %s succeed! \n", pTopic);
#endif                 
                return true;
            }
            
        } else {
#ifdef MQTT_DEBUG            
            debug("mqtt_sub_ack: wait for sub ack %d times \n", wait_for_suback_times);
#endif 
        }

        if (wait_for_suback_times >= (wait_for_suback_max-1)) {
#ifdef MQTT_DEBUG            
            debug("mqtt_subscribe: topic: %s failed! \n", pTopic);
#endif      
            wait_for_suback_times = 0;
            return false;
        }       
    } 

}




/**
*@name     mqtt_get_message()
*@brief	   
*@param		
*@return   state ok:true  state error: false
*/
void mqtt_get_message(bool* state)
{    
    static uint32_t timeout;
    
    if (getSn_SR(SOCK_TCPS) != SOCK_ESTABLISHED) { //! tcp un_established unexpectly
        uint8_t rc = getSn_SR(SOCK_TCPS);          //! confirm again
        if (rc != SOCK_ESTABLISHED) {
            debug("socket error : %02X \n", rc);        
            *state = false; 
            return;            
        }
    }   
    
    uint16_t len = getSn_RX_RSR(SOCK_TCPS);

    if (len > 8192) {  //! something wrong
        debug("len outside error %d \n", len);
        timeout = 0;
        *state = false;      
        
    } else if (len > 0) {
        vTaskSuspendAll();
        timeout = 0;
        recv(SOCK_TCPS, mqttDataBuf, len);
        if ((FIFO_RX_BUF_SIZE - fifoRxGetBufDataCount()) > (len + 1)) {
            fifoRxPushBuf(mqttDataBuf, len);
            
        } else {
            memset(mqttDataBuf, 0, sizeof(mqttDataBuf)); 
            debug("fifo_rx_buf full ... \n");
        }
        xTaskResumeAll();     
    
    } else {
        if (timeout++ > 5000) {
            debug("tcp no data within 5s \n");
            timeout = 0;
            *state = false;
        }
    }        
}




bool mqtt_disconnect(void)
{
    int buflen = sizeof(mqttTXBuf);
    int temp = MQTTSerialize_disconnect(mqttTXBuf, buflen);
    transport_sendPacketBuffer(mqttTXBuf, temp); 
    return transport_close();    
}



bool mqtt_pingreq(void)
{
    int buflen = sizeof(mqttTXBuf);
    int temp = MQTTSerialize_pingreq(mqttTXBuf, buflen);
    return transport_sendPacketBuffer(mqttTXBuf, temp);    
}



bool mqtt_puback(unsigned short packetid)
{
    int buflen = sizeof(mqttTXBuf);
    int temp = MQTTSerialize_puback(mqttTXBuf, buflen, packetid);
    return transport_sendPacketBuffer(mqttTXBuf, temp);
}




















