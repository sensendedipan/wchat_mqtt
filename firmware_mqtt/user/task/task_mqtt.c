

#include "main.h"
#include "debug.h"
#include "eeprom.h"
#include "task_mqtt.h"
#include "MQTTHelper.h"
#include "data_struct.h"
#include "message.h"
#include "dns.h"



uint8_t run_state = STATE_PHY;

void osTaskMqt(void *pParameters)
{
    static mqtt_mode_t mqtt_state = MQTT_MODE_IDLE;
    static volatile TickType_t currentTime;
    static volatile TickType_t currentTimeCheckPhy;
    static bool flagMqttRunning = true;
    static uint32_t  connectTryTimes = 0;
    static const uint32_t connectTryTimesMax = 240; //! about 4 minutes
    static uint32_t dhcpTryTimes = 0;
    const static uint32_t dhcpTryTimesMax = 500000;
    static uint32_t dnsTryTimes = 0;
    const static uint32_t dnsTryTimesMax = 600;//! about 10minutes
    static uint8_t wait_connect_ack_times = 0;  
    uint8_t serial_data;
    mqtt_message_t serial_msg;
    static char MQTT_CLIENT_ID[31] = {"HXGKCC"}; //! 最后一个字节作结束符
    static uint8_t client_id[12];
    
    while(1) {

        //!  check phy link state per 4s
        if (xTaskGetTickCount() - currentTimeCheckPhy >= 2000) { 
            currentTimeCheckPhy = xTaskGetTickCount(); 
            
            gFw5500LinkState = w5500CheckLink();
        }        
        
        switch (mqtt_state) { 
            case MQTT_MODE_IDLE:
                run_state = MQTT_MODE_IDLE;
                getMqttID(client_id);  
                convertHexToStr((uint8_t*)&MQTT_CLIENT_ID[6], client_id, sizeof(client_id));                
                mqtt_state = MQTT_MODE_INIT;   
                break;
            
            case MQTT_MODE_INIT:
                run_state = MQTT_MODE_INIT;
                w5500Init();
                vTaskDelay(1000); //!时间不能太短
                mqtt_state = MQTT_MODE_PHY;
                break;

            case MQTT_MODE_PHY: 
                run_state = STATE_PHY;
                debug("mqtt task: checking phy linking... \n");
                gFw5500LinkState = w5500CheckLink();
                if (gFw5500LinkState == true) { 
                    mqtt_state = MQTT_MODE_DHCP; 
                    
                } else {
                    debug("phy offline STATE_PHY\n");
                    vTaskDelay(2500);
                    mqtt_state = MQTT_MODE_PHY;
                }                
                break;
            
            case MQTT_MODE_DHCP:  //! 
                run_state = STATE_DHCP;
                debug("mqtt task: get ip from dhcp server or eeprom... \n");
            
                if (gFw5500LinkState == false) {  //! 时刻检测网线状态
                    vTaskDelay(2500);
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
                            vTaskDelay(100);
                            mqtt_state = MQTT_MODE_DHCP;
                        }
                    }
                    
                } else {
                    mqtt_state = MQTT_MODE_DNS;
                }                                   
                break;

            case MQTT_MODE_DNS:
                run_state = STATE_DNS;
                debug("mqtt task: get dns from dns server... \n");   
            
                if (gFw5500LinkState == false) {  //! 时刻检测网线状态                    
                    vTaskDelay(2500);
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
                            vTaskDelay(500);
                            mqtt_state = MQTT_MODE_DNS;
                        }                        
                    }                    
                    
                } else {
                    mqtt_state = MQTT_MODE_CONNECT;
                }
                
                break;
 
            case MQTT_MODE_CONNECT:
                run_state = STATE_CONNECT;
                debug("mqtt task: try to connect to mqtt server... \n");
            
                if (gFw5500LinkState == false) {  //! 时刻检测网线状态
                    vTaskDelay(2500);
                    mqtt_state = MQTT_MODE_PHY;
                    break;                    
                } 

                if (mqtt_connect(MQTT_CLIENT_ID, (char*)gEepromMlgi.usr, (char*)gEepromMlgi.psd) == true) { //! 链接已经建立，跳转到RUNNING模式
                    connectTryTimes = 0;                    
                    mqtt_state = MQTT_MODE_SUBSCRIBE;
                    break;

                } else if (connectTryTimes++ == connectTryTimesMax) { //! w5500 reinit per 4 minutes while connect failed
                    mqtt_state = MQTT_MODE_ETHREBOOT;
                
                } else if (connectTryTimes > connectTryTimesMax+1) {
                    systemReboot();
                    
                } else {
                    transport_close();
                    vTaskDelay(500);
                    mqtt_state = MQTT_MODE_CONNECT;
                }
                break;

            case MQTT_MODE_SUBSCRIBE:
                run_state = STATE_SUBCRIBE;
                debug("mqtt task: try to subcribe from mqtt server... \n");
            
                if (gFw5500LinkState == false) {  //! 时刻检测网线状态
                    vTaskDelay(2500);
                    mqtt_state = MQTT_MODE_PHY;
                    break;                    
                }
               
                //! 订阅响应主题
                if (mqtt_subscrib(TOPIC_ACK, MQTT_QOS0) == false) {
                    mqtt_state = MQTT_MODE_INIT;
                    break;
                } 

                //! 订阅报警主题
                if (mqtt_subscrib(TOPIC_ALARM, MQTT_QOS0) == false) {
                    mqtt_state = MQTT_MODE_INIT;
                    break;
                }

                //! 订阅状态主题
                if (mqtt_subscrib(TOPIC_STAE, MQTT_QOS0) == false) {
                    mqtt_state = MQTT_MODE_INIT;
                    break;
                }
                
                mqtt_state = MQTT_MODE_RUNNING;                
                break;
            
            case MQTT_MODE_RUNNING:
                run_state = STATE_RUNNING;
            
                if (gFw5500LinkState == false) {
                    vTaskDelay(2500);
                    mqtt_state = MQTT_MODE_PHY;
                    break;
                }

                //!  send pingreq(heartbeat) msg and check pingresp msg
                if (xTaskGetTickCount() - currentTime >= MQTT_KEEP_ALIVE*500) { 
                    currentTime = xTaskGetTickCount(); 
                    mqtt_pingreq();
                    debug("send a heartbeat...........\n");
                }
                
                //! data from serial to mqtt
                while (fifoGetBufDataCount()) {
                    fifoPopBuf(&serial_data, 1);
                    
                    if (msg_serial_parser(&serial_msg, serial_data)) {
                        handleSerialMessage(&serial_msg);
                    }
                }
                
                //! data from mqtt to serial                                
                mqtt_get_message(&flagMqttRunning);
                if (flagMqttRunning == false) {
                    flagMqttRunning = true;
                    mqtt_state = MQTT_MODE_INIT;
                }                               
                break;
            
            case MQTT_MODE_ETHREBOOT:
                run_state = MQTT_MODE_ETHREBOOT; 
                debug("turn to w5500 reboot \n");
                w5500Reboot();
                mqtt_state = MQTT_MODE_IDLE;
                break;
    
        }

        vTaskDelay(2);
    }
	   
}





void handleTopics(const char* recTopic, const char* pMessage, uint16_t len)
{           
    //usart1SendBuffer(pMessage, len);
    static uint32_t i;
    printf("apple %d \n", i++);
    ledFlashSet(0, 1, 10, 10);    
}

