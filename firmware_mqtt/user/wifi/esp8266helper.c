


#include "esp8266helper.h"



uint8_t  wifiRxBuf[WIFI_RX_BUF_MAX];


static uint8_t  wifiWaitingTime = 0;
static uint8_t  wifiTimerCnt = 0;  
static uint8_t  wifiTimerSeconds = 0;
static uint8_t  wifiTimerStart = 0;	
static uint8_t  wifiTimers = 0;
static uint16_t wifiBufIndex = 0;


void USART2_USART_IRQHandler() 
{
	unsigned char data;
	wifiTimerCnt = 0;
	
	if (USART_GetITStatus(USART2_USART, USART_IT_RXNE) != RESET) { 
		data = USART_ReceiveData(USART2_USART);  
		wifiRxBuf[wifiBufIndex] = data; 		
		wifiBufIndex++;                					
		
		if (wifiBufIndex > WIFI_RX_BUF_MAX) {      	
			wifiBufIndex = 0;
		} 
	}
}



void TIMER4_TIMER_IRQHandler()
{
	if (TIM_GetITStatus(TIMER4_TIMER, TIM_IT_Update) != RESET) {   
		TIM_ClearITPendingBit(TIMER4_TIMER, TIM_IT_Update);
		if (wifiTimerSeconds >= 1) wifiTimerSeconds++;	
		if (wifiTimerStart) wifiTimers++;
		
		if (wifiTimers > (wifiWaitingTime)) {
			wifiTimerStart = 0;
			wifiTimers = 0;
		}
		wifiTimerCnt++;	
	}
}




bool wifiFindStr(char *a)
{ 
	if (strstr((const char *)wifiRxBuf, (const char *)a) != NULL) {
		return true;

	} else {
		return false;
	}
}




void wifiClearWifiBuf(void)
{
	for (uint16_t i = 0; i < WIFI_RX_BUF_MAX; i++) {  
		wifiRxBuf[i] = 0X00;
	}
    wifiBufIndex = 0;
}




bool wifiSendCmd(char *cmd, char *ack, uint8_t holdTime)         
{
	bool res = false;

	wifiClearWifiBuf(); 
	
	for ( ; *cmd != '\0'; cmd++) {
		wifiSendChar(*cmd);
	}	
	wifiSendStr((uint8_t*)"\r\n");	
    
	if (holdTime == 0) return res;
	wifiTimers = 0;
	wifiWaitingTime = holdTime;
	wifiTimerStart = 1;
	
	while (wifiTimerStart&(!res)) {
        if (strstr((const char*)wifiRxBuf, (char*)ack) == NULL) {
             res = false;
            
        } else {
             res = true;
        }
	}
	return res;
}



bool wifiCmdCheck(char *ack, uint8_t holdTime)         
{
	bool res = false;

	wifiClearWifiBuf(); 	
    
	if (holdTime == 0) return res;
	wifiTimers = 0;
	wifiWaitingTime = holdTime;
	wifiTimerStart = 1;
	
	while (wifiTimerStart&(!res)) {
		if (strstr((const char*)wifiRxBuf, (char*)ack) == NULL) {
			 res = false;
			
		} else {
			 res = true;
		}
	}
    wifiClearWifiBuf(); 
	return res;
}



uint8_t convertStrToInt(uint8_t *p, uint8_t len)
{
	uint8_t i = 0;
	uint8_t value = 0;
	
	for (i = 0; i < len; i++) {
		value = value*10;
		value += (*(p++)-'0');
	}
	return value;
}



void swap(char *ch1, char *ch2)
{
	char tmp = *ch1;
	*ch1 = *ch2;
	*ch2 = tmp;
}



void convertIntToStr(int n, char str[])
{
	int i, len;
	
	for (i = 0; n != 0; ++i) {
		str[i] = n % 10 + '0';
		n /= 10;
	}
	str[i] = '\0';
	len = i;

	for (i = 0; i < len/2; ++i)
		swap(str+i, str+len-i-1);
	str[len] = '\0';
}


void convertHexToStr(uint8_t dest[], uint8_t src[], uint8_t len)
{
    uint8_t i = 0;
    uint8_t temp_h;
    uint8_t temp_l;
	
    for (i = 0; i < len; i++) {
        temp_h = src[i]>>4;
        temp_l = src[i]&0X0F;
       
		if (temp_h > 9) {
            dest[2*i] = (src[i]>>4) + 55;
			
		} else {
            dest[2*i] = (src[i]>>4) + 48;
		}
		
        if (temp_l > 9) {
            dest[2*i+1] = (src[i]&0X0F) + 55;
			
		} else {
            dest[2*i+1] = (src[i]&0X0F) + 48;
		}
    }
	
	dest[2*len] = '\0';
}



bool wifiSendData(uint8_t *buf, uint8_t len)
{
    bool flag = false;
    char size;
    char atcmd[40];  

    convertIntToStr(len, &size);
    strcpy(atcmd, "AT+CIPSEND=");
    strcat(atcmd, &size);
    
    if (wifiSendCmd(atcmd, ">", 2) == true) {    
        wifiSendBuf(buf, len);
        if (wifiCmdCheck("SEND OK", 2) == true) { 
            flag = true;
            
        } else {            
            flag = false;
        }
        
    } else {       
        flag = false;
    }   
    return flag;
}




void wifiSendDataPassthrough(uint8_t *buf, uint8_t len)
{
    wifiSendBuf(buf, len);
}




int wifiGetDataPassthrough(uint8_t *buf, uint8_t len)
{
    if (wifiBufIndex < len) {
        memcpy(buf, wifiRxBuf, wifiBufIndex);
        if (wifiBufIndex >= WIFI_RX_BUF_MAX) wifiBufIndex = 0;
        return wifiBufIndex;
        
    } else {
        memcpy(buf, wifiRxBuf, len);
        if (wifiBufIndex >= WIFI_RX_BUF_MAX) wifiBufIndex = 0;
        return len;
    }
    
}



























