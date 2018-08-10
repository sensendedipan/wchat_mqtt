

#include "mqtthelper.h"
#include "w5500_conf.h"
#include "debug.h"
#include "fiforx.h" 


static uint8_t  mqtt_broker_ip[4] = {0, 0, 0, 0};
static uint16_t mqtt_broker_port = 0;
static uint8_t buf_tcp_to_fifo[4096];

bool transport_open(void)
{ 
    int32_t ret;
    const uint32_t timeout = 5000;
    static uint16_t local_port = 5000;
    if (local_port >= 60000) local_port = 5000;
    memcpy(mqtt_broker_ip, gEepromMqtt.ip, 4);  
    mqtt_broker_port = gEepromMqtt.port;
    ret = socket(SOCK_TCPS, Sn_MR_TCP, local_port++, Sn_MR_ND);
    if (ret != 1) {      
        return false;
    }
    
    ret = connect(SOCK_TCPS, mqtt_broker_ip, mqtt_broker_port);
    if (ret != 1) {       
        return false;
        
    } 
        
    for (uint32_t i = 0; i < timeout; i++) {
        if (getSn_SR(SOCK_TCPS) == SOCK_ESTABLISHED) {
            debug("wait for %d times \n", i);
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
    close(SOCK_TCPS); 
    while(getSn_SR(SOCK_TCPS) != SOCK_CLOSED);  //! block
    return true;   
}               


/**
*@name     ethe_send_buffer()
*@brief	   
*@param		
*@return   
*/
bool ethe_send_buffer(char *pMsg, uint16_t msg_len)
{    
    if (send(SOCK_TCPS, (unsigned char*)pMsg, msg_len) == msg_len) {
        return true;
        
    } else {
        return false;
    }
}



/**
*@name     ethe_read_buffer()
*@brief	   
*@param		
*@return   state ok:true  state error: false
*/
void ethe_read_buffer(void)
{                           

    int32_t len = getSn_RX_RSR(SOCK_TCPS);
    if (len > sizeof(buf_tcp_to_fifo)) {
        debug("w5500 rc len outside: %d \n", len);
        vTaskSuspendAll();
        recv(SOCK_TCPS, buf_tcp_to_fifo, sizeof(buf_tcp_to_fifo));
        fifoRxPushBuf(buf_tcp_to_fifo, sizeof(buf_tcp_to_fifo));
        xTaskResumeAll();
        
    } else if (len > 0) {
        ledFlashSet(0, 1, 10, 10);
        vTaskSuspendAll();
        recv(SOCK_TCPS, buf_tcp_to_fifo, len);
        fifoRxPushBuf(buf_tcp_to_fifo, len);
        xTaskResumeAll();
    } 
}












