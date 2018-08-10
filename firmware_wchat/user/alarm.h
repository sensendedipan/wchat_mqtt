
#ifndef _ALARM_H
#define _ALARM_H


#define ALM_TYPE_DEV            0X05    //! 设备故障
#define ALM_CODE_DEV_DOOR       0X10    //! 柜门开启
#define ALM_CODE_DEV_RS485      0X50    //! 485通信故障
#define ALM_CODE_DEV_TCPIP      0X60    //! TCP通信故障
#define ALM_CODE_DEV_GPRS       0X70    //! GPRS数据断开
#define ALM_CODE_DEV_GPS_OK     0X00    //! GPS数据回复正常
#define ALM_CODE_DEV_GPS        0X80    //! GPS数据断开
#define ALM_CODE_DEV_POWOFF     0X90    //! 掉电报警
#define ALM_CODE_DEV_220V       0X90    //! 220V掉电
#define ALM_CODE_DEV_12DC       0X90    //! 12V掉电
//#define ALM_CODE_DEV_220V       0X01    //! 220V掉电  //! 220V 和12V掉电都统一报掉电报警
//#define ALM_CODE_DEV_12DC       0X02    //! 12V掉电
#define ALM_CODE_DEV_EEPROM     0X03    //! EEPROM发生故障
#define ALM_CODE_DEV_SW1        0X04    //! 开关1开启
#define ALM_CODE_DEV_SW2        0X05    //! 开关2开启
#define ALM_CODE_DEV_SW3        0X06    //! 开关3开启
#define ALM_CODE_DEV_3D         0X07    //! GPS未定位
#define ALM_CODE_DEV_SIMCARD    0X08    //! GPRS无卡
#define ALM_CODE_DEV_SWITCH     0X09    //! 简版手动开关

#define ALM_TYPE_SEN            0X02    //! 传感器故障
#define ALM_CODE_SEN_LINE_OK    0X00    //! 传感器断线
#define ALM_CODE_SEN_OFFLINE    0X01    //! 传感器断线
#define ALM_CODE_SEN_TEMP_OK    0X00    //! 温度正常
#define ALM_CODE_SEN_TEMP_H     0X11    //! 温度高报警
#define ALM_CODE_SEN_TEMP_HH    0X12    //! 温度高高报警
#define ALM_CODE_SEN_TEMP_L     0X13    //! 温度低报警
#define ALM_CODE_SEN_TEMP_LL    0X14    //! 温度低低报警
#define ALM_CODE_SEN_TEMP_EOR   0X15    //! 温度故障

#define ALM_CODE_SEN_HUMI_OK    0X00    //! 湿度正常
#define ALM_CODE_SEN_HUMI_H     0X21    //! 湿度高报警
#define ALM_CODE_SEN_HUMI_HH    0X22    //! 湿度高高报警                                                                    
#define ALM_CODE_SEN_HUMI_L     0X23    //! 湿度低报警
#define ALM_CODE_SEN_HUMI_LL    0X24    //! 湿度低低报警
#define ALM_CODE_SEN_HUMI_EOR   0X25    //! 湿度故障

#define ALM_CODE_SEN_WATER      0X31    //! 浸水报警
#define ALM_CODE_SEN_WATER_EOR  0X32    //! 浸水报警


#define ALM_TYPE_CHZ            0X03    //! 重合闸故障
#define ALM_CODE_CHZ_LINE_OK    0X00
#define ALM_CODE_CHZ_OFFLINE    0X05    //! 重合闸通信故障
#define ALM_CODE_CHZ_VOLT_OK    0X00    //! 电压正常
#define ALM_CODE_CHZ_VOLT_H     0X06    //! 电压高报警
#define ALM_CODE_CHZ_VOLT_HH    0X07    //! 电压高高报警
#define ALM_CODE_CHZ_VOLT_L     0X08    //! 电压低报警
#define ALM_CODE_CHZ_VOLT_LL    0X09    //! 电压低低报警

#define ALM_CODE_CHZ_CURR_OK    0X00    //! 电流正常
#define ALM_CODE_CHZ_CURR_H     0X0A    //! 电流高报警
#define ALM_CODE_CHZ_CURR_HH    0X0B    //! 电流高高报警

#define ALM_CODE_CHZ_LEAK_OK    0X00    //! 漏电正常
#define ALM_CODE_CHZ_LEAK_H     0X0C    //! 漏电高报警
#define ALM_CODE_CHZ_LEAK_HH    0X0D    //! 漏电高高报警


     

#define ALM_TYPE_PMU            0X01    //! 电源管理单元故障
#define ALM_CODE_PMU_LINE_OK    0X00    //! PMU通信故障
#define ALM_CODE_PMU_OFFLINE    0X01    //! PMU通信故障
#define ALM_CODE_PMU_VOLT1_OK   0X00    //! 第一路电压正常
#define ALM_CODE_PMU_VOLT1_H    0X14    //! 第一路电压高
#define ALM_CODE_PMU_VOLT1_HH   0X15    //! 第一路电压高高
#define ALM_CODE_PMU_VOLT1_L    0X16    //! 第一路电压低
#define ALM_CODE_PMU_VOLT1_LL   0X17    //! 第一路电压低低

#define ALM_CODE_PMU_CURR1_OK   0X00    //! 第一路电流正常
#define ALM_CODE_PMU_CURR1_H    0X18    //! 第一路电流高
#define ALM_CODE_PMU_CURR1_HH   0X19    //! 第一路电流高高

#define ALM_CODE_PMU_VOLT2_OK   0X00    //! 第二路电压正常
#define ALM_CODE_PMU_VOLT2_H    0X24    //! 第二路电压高
#define ALM_CODE_PMU_VOLT2_HH   0X25    //! 第二路电压高高
#define ALM_CODE_PMU_VOLT2_L    0X26    //! 第二路电压低
#define ALM_CODE_PMU_VOLT2_LL   0X27    //! 第二路电压低低

#define ALM_CODE_PMU_CURR2_OK   0X00    //! 第二路电流正常
#define ALM_CODE_PMU_CURR2_H    0X28    //! 第二路电流高
#define ALM_CODE_PMU_CURR2_HH   0X29    //! 第二路电流高高

#define ALM_CODE_PMU_VOLT3_OK   0X00    //! 第三路电压正常
#define ALM_CODE_PMU_VOLT3_H    0X34    //! 第三路电压高
#define ALM_CODE_PMU_VOLT3_HH   0X35    //! 第三路电压高高
#define ALM_CODE_PMU_VOLT3_L    0X36    //! 第三路电压低
#define ALM_CODE_PMU_VOLT3_LL   0X37    //! 第三路电压低低

#define ALM_CODE_PMU_CURR3_OK   0X00    //! 第三路电流正常
#define ALM_CODE_PMU_CURR3_H    0X38    //! 第三路电流高
#define ALM_CODE_PMU_CURR3_HH   0X39    //! 第三路电流高高

#define ALM_CODE_PMU_VOLT4_OK   0X00    //! 第四路电压正常
#define ALM_CODE_PMU_VOLT4_H    0X44    //! 第四路电压高
#define ALM_CODE_PMU_VOLT4_HH   0X45    //! 第四路电压高高
#define ALM_CODE_PMU_VOLT4_L    0X46    //! 第四路电压低
#define ALM_CODE_PMU_VOLT4_LL   0X47    //! 第四路电压低低

#define ALM_CODE_PMU_CURR4_OK   0X00    //! 第四路电流正常
#define ALM_CODE_PMU_CURR4_H    0X48    //! 第四路电流高
#define ALM_CODE_PMU_CURR4_HH   0X49    //! 第四路电流高高


#define ALM_CODE_PMU_VOLT5_OK   0X00    //! 第5路电压正常
#define ALM_CODE_PMU_VOLT5_H    0X54    //! 第5路电压高
#define ALM_CODE_PMU_VOLT5_HH   0X55    //! 第5路电压高高
#define ALM_CODE_PMU_VOLT5_L    0X56    //! 第5路电压低
#define ALM_CODE_PMU_VOLT5_LL   0X57    //! 第5路电压低低

#define ALM_CODE_PMU_CURR5_OK   0X00    //! 第5路电流正常
#define ALM_CODE_PMU_CURR5_H    0X58    //! 第5路电流高
#define ALM_CODE_PMU_CURR5_HH   0X59    //! 第5路电流高高

#define ALM_CODE_PMU_VOLT6_OK   0X00    //! 第6路电压正常
#define ALM_CODE_PMU_VOLT6_H    0X64    //! 第6路电压高
#define ALM_CODE_PMU_VOLT6_HH   0X65    //! 第6路电压高高
#define ALM_CODE_PMU_VOLT6_L    0X66    //! 第6路电压低
#define ALM_CODE_PMU_VOLT6_LL   0X67    //! 第6路电压低低

#define ALM_CODE_PMU_CURR6_OK   0X00    //! 第6路电流正常
#define ALM_CODE_PMU_CURR6_H    0X68    //! 第6路电流高
#define ALM_CODE_PMU_CURR6_HH   0X69    //! 第6路电流高高





typedef enum __mqtt_alarm_id_t
{    
    ALM_ID_GPS = 0,
    ALM_ID_TCP,
    ALM_ID_GPRS,
    ALM_ID_220V,
    ALM_ID_12DC,
    ALM_ID_EEPROM,
    ALM_ID_PMU,
    ALM_ID_CHZ,
    ALM_ID_DOOR,
    ALM_ID_SW1,
    ALM_ID_SW2,
    ALM_ID_SW3,
    ALM_ID_3D,
    ALM_ID_SIMCARD,
    ALM_ID_SWITCH,
    
    ALM_ID_SEN_OFFLINE,
    ALM_ID_SEN_TEMP_HH,
    ALM_ID_SEN_TEMP_H,
    ALM_ID_SEN_TEMP_LL,
    ALM_ID_SEN_TEMP_L,
    
    ALM_ID_SEN_HUMI_HH,
    ALM_ID_SEN_HUMI_H,
    ALM_ID_SEN_HUMI_LL,
    ALM_ID_SEN_HUMI_L,

    ALM_ID_CHZ_VOLT_OK,
    ALM_ID_CHZ_VOLT_HH,
    ALM_ID_CHZ_VOLT_H,
    ALM_ID_CHZ_VOLT_LL,
    ALM_ID_CHZ_VOLT_L,
    ALM_ID_CHZ_CURR_OK,
    ALM_ID_CHZ_CURR_HH,
    ALM_ID_CHZ_CURR_H,
    ALM_ID_CHZ_LEAK_OK,
    ALM_ID_CHZ_LEAK_HH,
    ALM_ID_CHZ_LEAK_H,
    
    ALM_ID_PMU_VOLT1_OK,
    ALM_ID_PMU_VOLT1_HH,
    ALM_ID_PMU_VOLT1_H,
    ALM_ID_PMU_VOLT1_LL,
    ALM_ID_PMU_VOLT1_L,
    ALM_ID_PMU_CURR1_OK,
    ALM_ID_PMU_CURR1_HH,
    ALM_ID_PMU_CURR1_H,

    ALM_ID_PMU_VOLT2_OK,
    ALM_ID_PMU_VOLT2_HH,
    ALM_ID_PMU_VOLT2_H,
    ALM_ID_PMU_VOLT2_LL,
    ALM_ID_PMU_VOLT2_L,
    ALM_ID_PMU_CURR2_OK,
    ALM_ID_PMU_CURR2_HH,
    ALM_ID_PMU_CURR2_H,
    
    ALM_ID_PMU_VOLT3_HH,
    ALM_ID_PMU_VOLT3_H,
    ALM_ID_PMU_VOLT3_LL,
    ALM_ID_PMU_VOLT3_L,
    ALM_ID_PMU_CURR3_HH,
    ALM_ID_PMU_CURR3_H,
    
    ALM_ID_PMU_VOLT4_HH,
    ALM_ID_PMU_VOLT4_H,
    ALM_ID_PMU_VOLT4_LL,
    ALM_ID_PMU_VOLT4_L,
    ALM_ID_PMU_CURR4_HH,
    ALM_ID_PMU_CURR4_H,
    
    ALM_ID_PMU_VOLT5_HH,
    ALM_ID_PMU_VOLT5_H,
    ALM_ID_PMU_VOLT5_LL,
    ALM_ID_PMU_VOLT5_L,
    ALM_ID_PMU_CURR5_HH,
    ALM_ID_PMU_CURR5_H,
    
    ALM_ID_PMU_VOLT6_HH,
    ALM_ID_PMU_VOLT6_H,
    ALM_ID_PMU_VOLT6_LL,
    ALM_ID_PMU_VOLT6_L,
    ALM_ID_PMU_CURR6_HH,
    ALM_ID_PMU_CURR6_H,
    
    ALM_ID_ALL,
 
} mqtt_alarm_id_t;




















#endif


























