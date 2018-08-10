


#include "rtc_driver.h"
#include "main.h"


bool rtcInit(void)  
{  
    RTC_InitTypeDef RTC_InitStructure;    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);    
    uint16_t retry = 0X1FFF; 
    
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x5050) {  
        RCC_LSEConfig(RCC_LSE_ON);     
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {  
            retry++;  
            delay_ms(10);
        }  
        if (retry == 0)return false;        
  
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);           
        RCC_RTCCLKCmd(ENABLE);     
  
        RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
        RTC_InitStructure.RTC_SynchPrediv  = 0xFF; 
        RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24; 
        RTC_Init(&RTC_InitStructure);  
  
        rtcSetTime(0, 0, 0); 
        rtcSetDate(18, 1, 1);         
  
        RTC_WriteBackupRegister(RTC_BKP_DR0, 0x5050);  
    }   
    return true;
}




void rtcSetDate(uint8_t year, uint8_t month, uint8_t day)  
{  
      
    RTC_DateTypeDef RTC_DateTypeInitStructure;  
      
    RTC_DateTypeInitStructure.RTC_Date = day;          
    RTC_DateTypeInitStructure.RTC_Month = month;          
    RTC_DateTypeInitStructure.RTC_Year = year; 
    
    RTC_SetDate(RTC_Format_BIN, &RTC_DateTypeInitStructure);  
} 




void rtcSetTime(uint8_t hour, uint8_t min, uint8_t sec)  
{  
    RTC_TimeTypeDef RTC_TimeTypeInitStructure;  
      
    RTC_TimeTypeInitStructure.RTC_Hours = hour;    
    RTC_TimeTypeInitStructure.RTC_Minutes = min;   
    RTC_TimeTypeInitStructure.RTC_Seconds = sec;   
    RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;        
      
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeTypeInitStructure);        
}



void rtcGetTime(gps_state_t *t)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;

    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

    t->gpsTime.hour = RTC_TimeStruct.RTC_Hours;
    t->gpsTime.minute  = RTC_TimeStruct.RTC_Minutes;
    t->gpsTime.second  = RTC_TimeStruct.RTC_Seconds;
    t->gpsDate.year = RTC_DateStruct.RTC_Year + 100;
    t->gpsDate.month  = RTC_DateStruct.RTC_Month - 1;
    t->gpsDate.day = RTC_DateStruct.RTC_Date;
}