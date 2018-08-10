#ifndef	__NTP_H__
#define	__NTP_H__

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

#include "types.h"
/* for ntpclient */ 

typedef signed char s_char;
typedef unsigned long long tstamp;
typedef unsigned int tdist;

#pragma pack(1)
typedef struct _NPTformat
{ 
    uint8_t dstaddr[4];     /* destination (local) address */
    char    version;        /* version number */
    char    leap;           /* leap indicator */
    char    mode;           /* mode */
    char    stratum;        /* stratum */
    char    poll;           /* poll interval */
    s_char  precision;      /* precision */
    tdist   rootdelay;      /* root delay */
    tdist   rootdisp;       /* root dispersion */
    char    refid;          /* reference ID */
    tstamp  reftime;        /* reference time */
    tstamp  org;            /* origin timestamp */
    tstamp  rec;            /* receive timestamp */
    tstamp  xmt;            /* transmit timestamp */
      
} NPTformat;
#pragma pack()


#pragma pack(1)
typedef struct _DateTime	/*�˽ṹ�嶨����NTPʱ��ͬ������ر���*/
{
  uint16_t year[2];			/*��Ϊ2�ֽ� ��Ϊ�ߵ���λ */
  uint8_t month;			/*	�� */
  uint8_t day;				/*	�� */
  uint8_t hour;				/*	ʱ */
  uint8_t minute;			/*	�� */
  uint8_t second;			/*	�� */
} DateTime;
#pragma pack()


#define SECS_PERDAY     	86400UL             	// seconds in a day = 60*60*24
#define UTC_ADJ_HRS         9              	        // SEOUL : GMT+9 
#define EPOCH               1900                    // NTP start year
void getSecondsFromNtpServer(uint8_t* buf, uint16_t idx);
void ntpClientInit(void);
void runNtpClient(void);
tstamp changeDateTimeToSeconds(void);
void calcDateTime(tstamp seconds);
#endif

