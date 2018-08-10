


#include "esp8266helper.h"
#include "esp8266.h"

static char networkAddr[80] = {0};

static uint8_t ip[4] = {192, 168, 0, 104};




void wifiInit(void)
{
    wifiSetServerIPAddr(false, gEepromMqtt.url, ip, gEepromMqtt.port, networkAddr);
}


void wifiSoftReset(void)
{
    wifiClearWifiBuf();
    if (true == wifiSendCmd("AT+RST", "OK", 50)) {
        debug("wifi reset ok \n");
        
    } else {
        debug("wifi reset failed \n");
    }
}



void wifiSetATE0(void)
{
	//wifiSendCmd("ATE0", "OK", 30);
}



bool wifiSmartlink(void)
{
    wifiExitTransfer();                      //检测是否在透传状态，如果是则退出透传状态

    uint8_t time = 0;
    while (time < 20) { //! 4s
        if (strstr((const char*)wifiRxBuf, (const char*)"WIFI GOT IP")) {
            return true;
            
        } else if (time < 20) {
            vTaskDelay(100);
            time++;
            
        } else {
            return false;
        }            
    }
   
}


void wifiExitTransfer(void)
{
	wifiClearWifiBuf();
    wifiSetATE0();
    
    if (true == wifiSendCmd("AT", "OK", 50)) { //! normal mode
        debug("wifi work in normal mode \n");
        
    } else {                                   //! transfer mode
        wifiSendStr((uint8_t*)"+++");          //! disable transfer mode
        debug("wifi is in transfer mode and try to turn to normal mode \n");
        vTaskDelay(500);
    }        
}



void wifiSetServerIPAddr(bool dns_enable, uint8_t url[50], uint8_t ip[4], uint16_t port, char addr[80])
{
    char wifiSerAddr[80] = {"AT+CIPSTART=\"TCP\",\""};
    
    char* ser_ip = NULL; //= inet_ntoa(ip[3] + (ip[2]<<8) + (ip[1]<<16) + (ip[0]<< 24));
    if (dns_enable == false) {
        ser_ip = inet_ntoa(ip[3] + (ip[2]<<8) + (ip[1]<<16) + (ip[0]<< 24));

    } else if (dns_enable == true) {
        ser_ip = (char*)url;

    } else {
        
    }
   
    strcat(wifiSerAddr, ser_ip);
    strcat(wifiSerAddr, "\",\"");
    char tempbuf[6] = {0}; 
    sprintf(tempbuf, "%d", port); 
    strcat(wifiSerAddr, tempbuf);
    strcat(wifiSerAddr, "\""); 
    memcpy(addr, wifiSerAddr, sizeof(wifiSerAddr));
}




void wifiConnectToServer(void)
{
    vTaskDelay(1000);
    if (wifiSendCmd("AT", "OK", 50) == true) { 
        debug("wifi test ok \n");
        
    } else {
        debug("wifi test failed \n");
    }

    vTaskDelay(1000);
    if (wifiSendCmd("AT+CWMODE=1", "OK", 50) == true) {  //! 设置8266为STA模式
        debug("wifi set station mode ok \n");
        
    } else {
        debug("wifi set station mode failed \n");
    }
    
    vTaskDelay(1000);
    if (wifiSendCmd("AT+CIPMUX=0", "OK", 50) == true) {  //! 关闭多路链接
        debug("wifi set cipmux mode ok \n");
        
    } else {
        debug("wifi set cipmux mode failed \n");
    } 

    vTaskDelay(1000);
    if (wifiSendCmd("AT+CWJAP=\"TP-LINK_6B0F\",\"hxgk8008\"", "WIFI GOT IP", 100) == true) {  //! 连接到指定WIFI
        debug("wifi connect to ap ok \n");
        
    } else {
        debug("wifi connect to ap failed \n");
    } 
    
//    if (wifiSendCmd((char*)networkAddr, "CONNECT", 50) == true) {  //! 连接到服务器
//        debug("wifi connect to server ok \n");
//        
//    } else {
//        debug("wifi connect to server failed \n");
//    }  AT+CIPSTART="TCP","192.168.0.103",8080

    vTaskDelay(1000);
    if (wifiSendCmd("AT+CIPSTART=\"TCP\",\"192.168.0.100\",\"5000\"", "OK", 50) == true) {  //! 连接到服务器
        debug("wifi connect to server ok \n");
        
    } else {
        debug("wifi connect to server failed \n");
    }
    
    if (wifiSendCmd("AT+CIPMODE=1", "OK", 50) == true) {  //! 开启透传
        debug("wifi set transfer mode ok \n");
        
    } else {
        debug("wifi set transfer mode failed \n");
    } 

    vTaskDelay(1000);
    if (wifiSendCmd("AT+CIPSEND", ">", 50) == true) {  //! 开启传输模式
        debug("wifi startup transfer ok \n");
        
    } else {
        debug("wifi startup transfer failed \n");
    }
    
    vTaskDelay(1000); //! 等待 > 符号
    wifiClearWifiBuf();
}




void wifiGetIp(void)
{
	static char *p1 = NULL;
    static char *p2 = NULL;
    static char ip[21] = {0};
    
	if (wifiSendCmd("AT+CIFSR", "OK", 30) == true) {
        p1 = strstr((const char*)wifiRxBuf, "\"");
        if (p1 != NULL) {
            p2 = strstr((const char*)(p1+1), "\"");
            if (p2 != NULL) {
                memcpy(ip, p1+1, p2-p1-1);
                printf("ip: %s\n", ip); 
            }
        }                         
	}	     
	wifiClearWifiBuf();
}











