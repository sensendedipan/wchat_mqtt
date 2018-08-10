/**
******************************************************************************
* @file   		ntp.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		pc������ΪNTP�����������ӣ�http://blog.csdn.net/tuesdayaa/article/details/78459714?locationNum=6&fps=1 
******************************************************************************
**/

#include <string.h>
#include <stdio.h>

#include "ntp.h"
#include "w5500_conf.h"
#include "types.h"
#include "socket.h"
#include "w5500.h"
#include "data_struct.h"
#include "main.h"


//#define NTP_DEBUG 


extern eeprom_add_t   gEepromAdd;
extern calendarData   gCalendar;
extern mqtt_time_state_t gTimeState;

static uint8_t ntp_ser_ips[10][4] = {
    {210,  72, 145,  44},
    {202, 120,   2, 101},
    {223,  65, 211,  46},
    {120,  25, 108,  11},
    {182,  92,  12,  11},
    {115,  28, 122, 198},
    {202, 112,  29,  82},
    {202, 108,   6,  95},
    {103, 107, 104, 231},
    {158,  69,  48,  97}};
        

static uint8_t ntp_port = 123; /*NTP�������˿ں�*/

static NPTformat ntpformat;
static DateTime nowdate;
static uint8_t ntp_message[48];
static tstamp total_seconds = 0;
static uint8_t time_zone = 39;

static uint8_t bufpub[512];                                         /*default is 2048*/
static uint8_t ntp_try_times = 0; 		                            /*�������*/




/**
*@brief		��NTP��������ȡʱ��
*@param		buf����Ż���
*@param		idx��������������ʼλ��
*@return	��
*/
void getSecondsFromNtpServer(uint8_t* buf, uint16_t idx)
{
	tstamp seconds = 0;

	for (uint8_t i = 0; i < 4; i++) {
		seconds = (seconds << 8) | buf[idx + i];
	}
    
	switch (time_zone)
	{
    case 0:
        seconds -= 12*3600;
        break;
    case 1:
        seconds -= 11*3600;
        break;
    case 2:
        seconds -= 10*3600;
        break;
    case 3:
        seconds -= (9*3600+30*60);
        break;
    case 4:
        seconds -= 9*3600;
        break;
    case 5:
    case 6:
        seconds -= 8*3600;
        break;
    case 7:
    case 8:
        seconds -= 7*3600;
        break;
    case 9:
    case 10:
        seconds -= 6*3600;
        break;
    case 11:
    case 12:
    case 13:
        seconds -= 5*3600;
        break;
    case 14:
        seconds -= (4*3600+30*60);
        break;
    case 15:
    case 16:
        seconds -= 4*3600;
        break;
    case 17:
        seconds -= (3*3600+30*60);
        break;
    case 18:
        seconds -= 3*3600;
        break;
    case 19:
        seconds -= 2*3600;
        break;
    case 20:
        seconds -= 1*3600;
        break;
    case 21:                           
    case 22:
        break;
    case 23:
    case 24:
    case 25:
        seconds += 1*3600;
        break;
    case 26:
    case 27:
        seconds += 2*3600;
        break;
    case 28:
    case 29:
        seconds += 3*3600;
        break;
    case 30:
        seconds += (3*3600+30*60);
        break;
    case 31:
        seconds += 4*3600;
        break;
    case 32:
        seconds += (4*3600+30*60);
        break;
    case 33:
        seconds += 5*3600;
        break;
    case 34:
        seconds += (5*3600+30*60);
        break;
    case 35:
        seconds += (5*3600+45*60);
        break;
    case 36:
        seconds += 6*3600;
        break;
    case 37:
        seconds += (6*3600+30*60);
        break;
    case 38:
        seconds += 7*3600;
        break;
    case 39:
        seconds += 8*3600;
        break;
    case 40:
        seconds += 9*3600;
        break;
    case 41:
        seconds += (9*3600+30*60);
        break;
    case 42:
        seconds += 10*3600;
        break;
    case 43:
        seconds += (10*3600+30*60);
        break;
    case 44:
        seconds += 11*3600;
        break;
    case 45:
        seconds += (11*3600+30*60);
        break;
    case 46:
        seconds += 12*3600;
        break;
    case 47:
        seconds += (12*3600+45*60);
        break;
    case 48:
        seconds += 13*3600;
        break;
    case 49:
        seconds += 14*3600;
        break;
	}
	
	total_seconds = seconds;
	calcDateTime(seconds); 																									/*��UTCʱ���������*/
}




/**
*@brief		��ʼ��NTP Client��Ϣ
*@param		��
*@return	��
*/
void ntpClientInit(void)
{
	uint8_t flag;
	ntpTimerInit();
	ntpformat.dstaddr[0] = gEepromAdd.ntp[0];
	ntpformat.dstaddr[1] = gEepromAdd.ntp[1];
	ntpformat.dstaddr[2] = gEepromAdd.ntp[2];
	ntpformat.dstaddr[3] = gEepromAdd.ntp[3];
		
	ntpformat.leap = 0;          /* leap indicator */ 
	ntpformat.version = 4;       /* version number */
	ntpformat.mode = 3;          /* mode */
	ntpformat.stratum = 0;       /* stratum */
	ntpformat.poll = 0;          /* poll interval */
	ntpformat.precision = 0;     /* precision */
	ntpformat.rootdelay = 0;     /* root delay */
	ntpformat.rootdisp = 0;      /* root dispersion */
	ntpformat.refid = 0;         /* reference ID */
	ntpformat.reftime = 0;       /* reference time */
	ntpformat.org = 0;           /* origin timestamp */
	ntpformat.rec = 0;           /* receive timestamp */
	ntpformat.xmt = 1;           /* transmit timestamp */
	
	flag = (ntpformat.leap<<6) + (ntpformat.version<<3) + ntpformat.mode; //one byte Flag
	memcpy(ntp_message, (void const*)(&flag), 1);
}



/**
*@brief			ִ��NTP Client������
*@param			��
*@return		��
*/

#define NTP_UPDATE_TIME   10800000 //! ÿ6СʱУʱһ��
void runNtpClient(void)
{
    static bool flagNtpInited = false;
    static volatile TickType_t currentTime;
    if (xTaskGetTickCount() - currentTime >= NTP_UPDATE_TIME) { 
        currentTime = xTaskGetTickCount();
        flagNtpInited = false;
        total_seconds = 0;
    }
    
    if (flagNtpInited == false) {
        ntpClientInit();
        flagNtpInited = true;
    }
    
	if (total_seconds > 0) {
        return;
        
    } else {							                                        /*�ѻ�ȡʱ������ִ��NPT����*/
		uint16_t len;
		uint8_t* data_buf = bufpub;
		uint32_t destip = 0;
		uint16_t destport;
		uint16_t startindex = 40; 
        
        /*�ظ�����ʱ�������׵�ַ*/
		switch (getSn_SR(SOCK_NTP))
		{
        case SOCK_UDP:	 				                                        /*UDPģʽ����*/
            if (total_seconds > 0) return;	                                    /*�ѻ�ȡʱ������ִ��NPT����*/
            
            if (ntp_try_times < 100) { 
              
                if (ntptimer > 0) {			                                    /*1������һ��*/
                                                                                /*��������NTP������*/
                    //sendto(SOCK_NTP, ntp_message, sizeof(ntp_message), ntp_server_ip, ntp_port);
                    if (ntp_try_times < 10) {
                        sendto(SOCK_NTP, ntp_message, sizeof(ntp_message), gEepromAdd.ntp, ntp_port);
                        
                    } else {
                        sendto(SOCK_NTP, ntp_message, sizeof(ntp_message), ntp_ser_ips[ntp_try_times%10], ntp_port);
                    }
                    
                    ntp_try_times++;
#ifdef NTP_DEBUG
                    printf("ntp_try_times:%d \r\n", ntp_try_times);	        /*��ӡ�������*/
#endif    
                    ntptimer = 0;									 	        /*�ڶ�ʱ��5�ж��м�1*/
                }
                
            } else  {
                ntp_try_times = 0;
            }
            
            if ((len = getSn_RX_RSR(SOCK_NTP)) > 0) {
                if (len > TX_RX_MAX_BUF_SIZE) len = TX_RX_MAX_BUF_SIZE;	
                recvfrom(SOCK_NTP, data_buf, len, (uint8_t*)&destip, &destport);/*����NTP�������ظ�����*/
                getSecondsFromNtpServer(data_buf, startindex);				    /*��NTP��������ȡʱ��*/
                
                gCalendar.date.year  = (nowdate.year[0]<<8)+nowdate.year[1];
                gCalendar.date.month = nowdate.month;
                gCalendar.date.day   = nowdate.day;
                gCalendar.time.hour  = nowdate.hour;
                gCalendar.time.minute= nowdate.minute;
                gCalendar.time.second= nowdate.second;
                
#ifdef NTP_DEBUG
                printf("����ʱ�䣺%d-%02d-%02d %02d:%02d:%02d\r\n",  
                (nowdate.year[0]<<8)+nowdate.year[1],							
                nowdate.month,
                nowdate.day,
                nowdate.hour,
                nowdate.minute,
                nowdate.second);
#endif
                
                if ((((nowdate.year[0]<<8)+nowdate.year[1]) > 2017)&&(((nowdate.year[0]<<8)+nowdate.year[1]) < 2037)) {  //! ��Ч��20��
                    gTimeState.time = timeToStamp(gCalendar);
                    gTimeState.valid = true;        
                } 
                ntp_try_times = 0;
            }
            break;
        case SOCK_CLOSED: 																							
            socket(SOCK_NTP, Sn_MR_UDP, ntp_port, 0);
            break;
        default: 
            break;
		}
	} 
}

/**
*@brief			��������ʱ��
*@param			seconds�UUTC �����׼ʱ�� 
*@return		��
*/
void calcDateTime(tstamp seconds)
{
	uint8_t yf = 0;
	uint32_t p_year_total_sec;
	uint32_t r_year_total_sec;
	tstamp n = 0, d = 0, total_d = 0, rz = 0;
	uint16_t y = 0, r = 0, yr = 0;
	signed long long yd = 0;
	
	n = seconds;
	total_d = seconds/(SECS_PERDAY);
	d = 0;
	p_year_total_sec=SECS_PERDAY*365;
	r_year_total_sec=SECS_PERDAY*366;
    
	while (n >= p_year_total_sec) {
		if ((EPOCH+r)%400==0 || ((EPOCH+r)%100!=0 && (EPOCH+r)%4==0)) {
			n = n - (r_year_total_sec);
			d = d + 366;
            
		} else {
			n = n - (p_year_total_sec);
			d = d + 365;
		}
		r += 1;
		y += 1;

	}

	y += EPOCH;

	nowdate.year[0] = (uint8_t)((y & 0xff00)>>8);
	nowdate.year[1] = (uint8_t)(y & 0xff);

	yd = 0;
	yd = total_d - d;

	yf = 1;
	while (yd >= 28) {
		if (yf==1 || yf==3 || yf==5 || yf==7 || yf==8 || yf==10 || yf==12) {
			yd -= 31;
			if (yd < 0) break;
			rz += 31;
		}
		
		if (yf == 2) {
			if (y%400==0 || (y%100!=0 && y%4==0)) {
				yd -= 29;
				if (yd < 0) break;
				rz += 29;
                
			} else {
				yd -= 28;
				if (yd < 0) break;
				rz += 28;
			}
		} 
        
		if (yf==4 || yf==6 || yf==9 || yf==11) {
			yd -= 30;
			if (yd < 0) break;
			rz += 30;
		}
        
		yf += 1;
	}
	nowdate.month = yf;
	yr = total_d-d-rz;
	
	yr += 1;
	
	nowdate.day = yr;
	
	//calculation for time
	seconds = seconds%SECS_PERDAY;
	nowdate.hour = seconds/3600;
	nowdate.minute = (seconds%3600)/60;
	nowdate.second = (seconds%3600)%60;

}



/**
*@brief		�ı�����ʱ��Ϊ��
*@param	    seconds�� 
*@return	��
*/
tstamp changeDateTimeToSeconds(void) 
{
	tstamp seconds = 0;
	uint32_t total_day = 0;
	uint16_t i = 0, run_year_cnt = 0, l = 0;
	
	l = nowdate.year[0];//high
	
	l = (l<<8);
	
	l = l + nowdate.year[1];//low
	
	for (i = EPOCH; i < l; i++)	{
		if ((i%400==0) || ((i%100!=0) && (i%4==0))) {
			run_year_cnt += 1;
		}
	}
	
	total_day = (l-EPOCH-run_year_cnt)*365 + run_year_cnt*366;
	
	for (i = 1; i <= nowdate.month; i++) {
		if (i==5 || i==7 || i==10 || i==12) {
			total_day += 30;
		}
        
		if (i == 3) {
			if (l%400==0 && l%100!=0 && l%4==0) {
				total_day += 29;
                
			} else {
				total_day += 28;
			}
		} 
        
		if (i==2 || i==4 || i==6 || i==8 || i==9 || i==11) {
			total_day += 31;
		}
	}
	
	seconds = (total_day+nowdate.day-1)*24*3600;  
	seconds += nowdate.second;
	seconds += nowdate.minute*60;
	seconds += nowdate.hour*3600;
	
	return seconds;
}
