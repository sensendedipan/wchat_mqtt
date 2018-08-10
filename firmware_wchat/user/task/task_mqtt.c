

#include "main.h"
#include "debug.h"
#include "eeprom.h"
#include "task_mqtt.h"
#include "MQTTHelper.h"
#include "data_struct.h"
#include "dns.h"

extern uint8_t run_state;


void osTaskMqt(void *pParameters)
{
    static mqtt_mode_t mqtt_state = MQTT_MODE_IDLE;
    static volatile TickType_t currentTime;
    static bool flagMqttRunning = true;
    static uint32_t  connectTryTimes = 0;
    static const uint32_t connectTryTimesMax = 600; //! about 10 minutes
    static uint32_t dhcpTryTimes = 0;
    const static uint32_t dhcpTryTimesMax = 500000;
    static uint32_t dnsTryTimes = 0;
    const static uint32_t dnsTryTimesMax = 600;//! about 10minutes
    static uint8_t wait_connect_ack_times = 0;  
    static uint8_t buf_usart_to_tcp[3072];
    static uint16_t rc;
    static uint8_t msg_handshake[21] = {0};
    static bool flag_send_msg_handshake = false;
    
    while(1) { 
       
        switch (mqtt_state) { 
            case MQTT_MODE_IDLE: 
                msg_handshake[0]  = 0XFA;
                msg_handshake[1]  = 0XFB;
                msg_handshake[2]  = 0XFC;
                msg_handshake[3]  = 0XFD;
                msg_handshake[4]  = 0X00;
                msg_handshake[5]  = 0X00;
                msg_handshake[6]  = 0XFE;
                msg_handshake[7]  = 0XFF;
                msg_handshake[8]  = 0XFF;
                msg_handshake[9]  = 0X06;
                msg_handshake[10] = 0X00;
                msg_handshake[11] = 0X00;
                msg_handshake[12] = 0X00;
                msg_handshake[13] = myMac[0];
                msg_handshake[14] = myMac[1];
                msg_handshake[15] = myMac[2];
                msg_handshake[16] = myMac[3];
                msg_handshake[17] = myMac[4];
                msg_handshake[18] = myMac[5];
                msg_handshake[19] = (uint8_t)(crc16((const uint8_t*)msg_handshake, 19));        //! 低字节
                msg_handshake[20] = (uint8_t)(crc16((const uint8_t*)msg_handshake, 19) >> 8);   //! 高字节
                mqtt_state = MQTT_MODE_INIT;   
                break;
            
            case MQTT_MODE_INIT:               
                w5500Init();
                vTaskDelay(1000); //!时间不能太短
                mqtt_state = MQTT_MODE_PHY;
                break;

            case MQTT_MODE_PHY: 
                run_state = STATE_PHY;
                if (gFw5500LinkState == true) { 
                    mqtt_state = MQTT_MODE_DHCP; 
                    
                } else {
                    vTaskDelay(5000);
                    mqtt_state = MQTT_MODE_PHY;
                }                
                break;
            
            case MQTT_MODE_DHCP:  //! 
                run_state = STATE_DHCP;
                if (gFw5500LinkState == false) {  //! 时刻检测网线状态
                    vTaskDelay(5000);
                    mqtt_state = MQTT_MODE_PHY;
                    break;
                    
                }
                
                if (gEepromLnet.dhcp == IP_DYNAMIC) {
                    if (runDHCPClient()) {
                        gFlagDHCPState = false;
                        dhcpTryTimes = 0;
                        mqtt_state = MQTT_MODE_DNS;
                    
                    } else {
                        
                        if (dhcpTryTimes++ == dhcpTryTimesMax) {                            
                            mqtt_state = MQTT_MODE_ETHREBOOT;
                            
                        } else if (dhcpTryTimes++ >= dhcpTryTimesMax+100) { 
                            dhcpTryTimes = 0;
                            systemReboot();
                        
                        } else {
                            vTaskDelay(200);
                            mqtt_state = MQTT_MODE_DHCP;
                        }
                    }
                    
                } else {
                    mqtt_state = MQTT_MODE_DNS;
                }                                   
                break;

            case MQTT_MODE_DNS:
                run_state = STATE_DNS; 
                if (gFw5500LinkState == false) {  //! 时刻检测网线状态
                    vTaskDelay(5000);
                    mqtt_state = MQTT_MODE_PHY;
                    break;
                    
                }
                
                if (gEepromMqtt.dns == DNS_ENABLE) { 
                    if (runDNS(gEepromMqtt.url) == true) {
                        dnsTryTimes = 0;   
                        mqtt_state = MQTT_MODE_CONNECT; 
                        
                    } else { 
                        if (dnsTryTimes++ > dnsTryTimesMax) {
                            dnsTryTimes = 0;
                            mqtt_state = MQTT_MODE_ETHREBOOT;
                            
                        } else {
                            vTaskDelay(1000);
                            mqtt_state = MQTT_MODE_DNS;
                        }                        
                    }                    
                    
                } else {
                    mqtt_state = MQTT_MODE_CONNECT;
                }
                
                break;
 
            case MQTT_MODE_CONNECT:
                run_state = STATE_CONNECT;
                flag_send_msg_handshake = false;
            
                if (gFw5500LinkState == false) {  //! 时刻检测网线状态
                    vTaskDelay(5000);
                    mqtt_state = MQTT_MODE_PHY;
                    break;                    
                } 
                
                if (getSn_SR(SOCK_TCPS) == SOCK_ESTABLISHED) { //! 链接已经建立，跳转到RUNNING模式
                    connectTryTimes = 0;
                    mqtt_state = MQTT_MODE_RUNNING;
                    break;
                    
                } else if (transport_open() == true) {
                    connectTryTimes = 0;
                    mqtt_state = MQTT_MODE_RUNNING;
                    break;

                } else if (connectTryTimes++ > connectTryTimesMax) {
                    connectTryTimes = 0;
                    mqtt_state = MQTT_MODE_ETHREBOOT;
                
                } else {
                    transport_close();
                    vTaskDelay(2000);
                    mqtt_state = MQTT_MODE_CONNECT;
                }
                break;
                
            case MQTT_MODE_RUNNING:
                run_state = STATE_RUNNING;
                if (gFw5500LinkState == false) {
                    debug("running: phy offline \n");
                    mqtt_state = MQTT_MODE_PHY;
                    break;
                }
                
                uint8_t sr = getSn_SR(SOCK_TCPS);
                if (sr == SOCK_CLOSED) {
                    debug("running: socket closed \n");
                    mqtt_state = MQTT_MODE_CONNECT;
                    break;
                    
                } else if (sr == SOCK_CLOSE_WAIT) {
                    debug("running: socket close wait \n");
                    transport_close();
                    mqtt_state = MQTT_MODE_CONNECT;
                    break;
                    
                } else if (sr == SOCK_SYNSENT) {
                    debug("running: socket synsent \n");
                    if (wait_connect_ack_times++ > 100) {
                        wait_connect_ack_times = 0;
                        mqtt_state = MQTT_MODE_CONNECT;
                    
                    } else {
                        vTaskDelay(1);
                    }
                    
                    break;
                    
                } else {
                    if (flag_send_msg_handshake == false) {
                        if (ethe_send_buffer((char*)msg_handshake, sizeof(msg_handshake)) == false) {                          
                            mqtt_state = MQTT_MODE_CONNECT;
                            
                        } else {
                            flag_send_msg_handshake = true;
                        }
                    }
                }
                  
                rc = fifoGetBufDataCount();
             
                if (rc > sizeof(buf_usart_to_tcp)) {
                    vTaskSuspendAll();                     
                    fifoPopBuf(buf_usart_to_tcp, sizeof(buf_usart_to_tcp));
                    if (ethe_send_buffer((char*)buf_usart_to_tcp, sizeof(buf_usart_to_tcp)) == false) {
                        mqtt_state = MQTT_MODE_CONNECT;                         
                    }
                    xTaskResumeAll();
                    
                } else if (rc > 0) {
                    vTaskSuspendAll();
                    fifoPopBuf(buf_usart_to_tcp, rc);
                    ledFlashSet(0, 1, 10, 10);
                    if (ethe_send_buffer((char*)buf_usart_to_tcp, rc) == false) {                          
                        mqtt_state = MQTT_MODE_CONNECT;                         
                    } 
                    xTaskResumeAll();                    
                }                                     
                
                ethe_read_buffer();

                break;
            
            case MQTT_MODE_ETHREBOOT:     
                w5500Reboot();
                mqtt_state = MQTT_MODE_IDLE;
                break;
    
        }

        vTaskDelay(1);
    }
	   
}


