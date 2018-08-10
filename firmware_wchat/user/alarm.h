
#ifndef _ALARM_H
#define _ALARM_H


#define ALM_TYPE_DEV            0X05    //! �豸����
#define ALM_CODE_DEV_DOOR       0X10    //! ���ſ���
#define ALM_CODE_DEV_RS485      0X50    //! 485ͨ�Ź���
#define ALM_CODE_DEV_TCPIP      0X60    //! TCPͨ�Ź���
#define ALM_CODE_DEV_GPRS       0X70    //! GPRS���ݶϿ�
#define ALM_CODE_DEV_GPS_OK     0X00    //! GPS���ݻظ�����
#define ALM_CODE_DEV_GPS        0X80    //! GPS���ݶϿ�
#define ALM_CODE_DEV_POWOFF     0X90    //! ���籨��
#define ALM_CODE_DEV_220V       0X90    //! 220V����
#define ALM_CODE_DEV_12DC       0X90    //! 12V����
//#define ALM_CODE_DEV_220V       0X01    //! 220V����  //! 220V ��12V���綼ͳһ�����籨��
//#define ALM_CODE_DEV_12DC       0X02    //! 12V����
#define ALM_CODE_DEV_EEPROM     0X03    //! EEPROM��������
#define ALM_CODE_DEV_SW1        0X04    //! ����1����
#define ALM_CODE_DEV_SW2        0X05    //! ����2����
#define ALM_CODE_DEV_SW3        0X06    //! ����3����
#define ALM_CODE_DEV_3D         0X07    //! GPSδ��λ
#define ALM_CODE_DEV_SIMCARD    0X08    //! GPRS�޿�
#define ALM_CODE_DEV_SWITCH     0X09    //! ����ֶ�����

#define ALM_TYPE_SEN            0X02    //! ����������
#define ALM_CODE_SEN_LINE_OK    0X00    //! ����������
#define ALM_CODE_SEN_OFFLINE    0X01    //! ����������
#define ALM_CODE_SEN_TEMP_OK    0X00    //! �¶�����
#define ALM_CODE_SEN_TEMP_H     0X11    //! �¶ȸ߱���
#define ALM_CODE_SEN_TEMP_HH    0X12    //! �¶ȸ߸߱���
#define ALM_CODE_SEN_TEMP_L     0X13    //! �¶ȵͱ���
#define ALM_CODE_SEN_TEMP_LL    0X14    //! �¶ȵ͵ͱ���
#define ALM_CODE_SEN_TEMP_EOR   0X15    //! �¶ȹ���

#define ALM_CODE_SEN_HUMI_OK    0X00    //! ʪ������
#define ALM_CODE_SEN_HUMI_H     0X21    //! ʪ�ȸ߱���
#define ALM_CODE_SEN_HUMI_HH    0X22    //! ʪ�ȸ߸߱���                                                                    
#define ALM_CODE_SEN_HUMI_L     0X23    //! ʪ�ȵͱ���
#define ALM_CODE_SEN_HUMI_LL    0X24    //! ʪ�ȵ͵ͱ���
#define ALM_CODE_SEN_HUMI_EOR   0X25    //! ʪ�ȹ���

#define ALM_CODE_SEN_WATER      0X31    //! ��ˮ����
#define ALM_CODE_SEN_WATER_EOR  0X32    //! ��ˮ����


#define ALM_TYPE_CHZ            0X03    //! �غ�բ����
#define ALM_CODE_CHZ_LINE_OK    0X00
#define ALM_CODE_CHZ_OFFLINE    0X05    //! �غ�բͨ�Ź���
#define ALM_CODE_CHZ_VOLT_OK    0X00    //! ��ѹ����
#define ALM_CODE_CHZ_VOLT_H     0X06    //! ��ѹ�߱���
#define ALM_CODE_CHZ_VOLT_HH    0X07    //! ��ѹ�߸߱���
#define ALM_CODE_CHZ_VOLT_L     0X08    //! ��ѹ�ͱ���
#define ALM_CODE_CHZ_VOLT_LL    0X09    //! ��ѹ�͵ͱ���

#define ALM_CODE_CHZ_CURR_OK    0X00    //! ��������
#define ALM_CODE_CHZ_CURR_H     0X0A    //! �����߱���
#define ALM_CODE_CHZ_CURR_HH    0X0B    //! �����߸߱���

#define ALM_CODE_CHZ_LEAK_OK    0X00    //! ©������
#define ALM_CODE_CHZ_LEAK_H     0X0C    //! ©��߱���
#define ALM_CODE_CHZ_LEAK_HH    0X0D    //! ©��߸߱���


     

#define ALM_TYPE_PMU            0X01    //! ��Դ����Ԫ����
#define ALM_CODE_PMU_LINE_OK    0X00    //! PMUͨ�Ź���
#define ALM_CODE_PMU_OFFLINE    0X01    //! PMUͨ�Ź���
#define ALM_CODE_PMU_VOLT1_OK   0X00    //! ��һ·��ѹ����
#define ALM_CODE_PMU_VOLT1_H    0X14    //! ��һ·��ѹ��
#define ALM_CODE_PMU_VOLT1_HH   0X15    //! ��һ·��ѹ�߸�
#define ALM_CODE_PMU_VOLT1_L    0X16    //! ��һ·��ѹ��
#define ALM_CODE_PMU_VOLT1_LL   0X17    //! ��һ·��ѹ�͵�

#define ALM_CODE_PMU_CURR1_OK   0X00    //! ��һ·��������
#define ALM_CODE_PMU_CURR1_H    0X18    //! ��һ·������
#define ALM_CODE_PMU_CURR1_HH   0X19    //! ��һ·�����߸�

#define ALM_CODE_PMU_VOLT2_OK   0X00    //! �ڶ�·��ѹ����
#define ALM_CODE_PMU_VOLT2_H    0X24    //! �ڶ�·��ѹ��
#define ALM_CODE_PMU_VOLT2_HH   0X25    //! �ڶ�·��ѹ�߸�
#define ALM_CODE_PMU_VOLT2_L    0X26    //! �ڶ�·��ѹ��
#define ALM_CODE_PMU_VOLT2_LL   0X27    //! �ڶ�·��ѹ�͵�

#define ALM_CODE_PMU_CURR2_OK   0X00    //! �ڶ�·��������
#define ALM_CODE_PMU_CURR2_H    0X28    //! �ڶ�·������
#define ALM_CODE_PMU_CURR2_HH   0X29    //! �ڶ�·�����߸�

#define ALM_CODE_PMU_VOLT3_OK   0X00    //! ����·��ѹ����
#define ALM_CODE_PMU_VOLT3_H    0X34    //! ����·��ѹ��
#define ALM_CODE_PMU_VOLT3_HH   0X35    //! ����·��ѹ�߸�
#define ALM_CODE_PMU_VOLT3_L    0X36    //! ����·��ѹ��
#define ALM_CODE_PMU_VOLT3_LL   0X37    //! ����·��ѹ�͵�

#define ALM_CODE_PMU_CURR3_OK   0X00    //! ����·��������
#define ALM_CODE_PMU_CURR3_H    0X38    //! ����·������
#define ALM_CODE_PMU_CURR3_HH   0X39    //! ����·�����߸�

#define ALM_CODE_PMU_VOLT4_OK   0X00    //! ����·��ѹ����
#define ALM_CODE_PMU_VOLT4_H    0X44    //! ����·��ѹ��
#define ALM_CODE_PMU_VOLT4_HH   0X45    //! ����·��ѹ�߸�
#define ALM_CODE_PMU_VOLT4_L    0X46    //! ����·��ѹ��
#define ALM_CODE_PMU_VOLT4_LL   0X47    //! ����·��ѹ�͵�

#define ALM_CODE_PMU_CURR4_OK   0X00    //! ����·��������
#define ALM_CODE_PMU_CURR4_H    0X48    //! ����·������
#define ALM_CODE_PMU_CURR4_HH   0X49    //! ����·�����߸�


#define ALM_CODE_PMU_VOLT5_OK   0X00    //! ��5·��ѹ����
#define ALM_CODE_PMU_VOLT5_H    0X54    //! ��5·��ѹ��
#define ALM_CODE_PMU_VOLT5_HH   0X55    //! ��5·��ѹ�߸�
#define ALM_CODE_PMU_VOLT5_L    0X56    //! ��5·��ѹ��
#define ALM_CODE_PMU_VOLT5_LL   0X57    //! ��5·��ѹ�͵�

#define ALM_CODE_PMU_CURR5_OK   0X00    //! ��5·��������
#define ALM_CODE_PMU_CURR5_H    0X58    //! ��5·������
#define ALM_CODE_PMU_CURR5_HH   0X59    //! ��5·�����߸�

#define ALM_CODE_PMU_VOLT6_OK   0X00    //! ��6·��ѹ����
#define ALM_CODE_PMU_VOLT6_H    0X64    //! ��6·��ѹ��
#define ALM_CODE_PMU_VOLT6_HH   0X65    //! ��6·��ѹ�߸�
#define ALM_CODE_PMU_VOLT6_L    0X66    //! ��6·��ѹ��
#define ALM_CODE_PMU_VOLT6_LL   0X67    //! ��6·��ѹ�͵�

#define ALM_CODE_PMU_CURR6_OK   0X00    //! ��6·��������
#define ALM_CODE_PMU_CURR6_H    0X68    //! ��6·������
#define ALM_CODE_PMU_CURR6_HH   0X69    //! ��6·�����߸�





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


























