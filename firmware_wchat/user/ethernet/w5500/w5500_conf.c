/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>
#include "debug.h"
#include "board.h"
#include "eeprom.h"
#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"
#include "timer3_driver.h"


#define W5500_DESELECT()        GPIO_SetBits(W5500_GPIO_REG_CS,   W5500_GPIO_PIN_CS)
#define W5500_SELECT()          GPIO_ResetBits(W5500_GPIO_REG_CS, W5500_GPIO_PIN_CS)


uint8_t myMac[6] = {0X4A, 0x4B, 0x17, 0x7D, 0xF9, 0xB6};
			

uint32_t	ntptimer  = 0;										/*NPT�����*/

void w5500Delay(void)
{
    const uint32_t delayTime = 100;
    
    for (uint32_t i = 0; i < delayTime; i++);
}



/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void w5500GpioConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(W5500_RCC_AHB1Periph, ENABLE);

	//! power ctrl
	GPIO_InitStructure.GPIO_Pin = W5500_GPIO_PIN_CTRL;					 		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		 
	GPIO_Init(W5500_GPIO_REG_CTRL, &GPIO_InitStructure);
    
    GPIO_SetBits(W5500_GPIO_REG_CTRL, W5500_GPIO_PIN_CTRL);
   

	//! w5500 cs
	GPIO_InitStructure.GPIO_Pin = W5500_GPIO_PIN_CS;							 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		 
	GPIO_Init(W5500_GPIO_REG_CS, &GPIO_InitStructure);							 
	GPIO_SetBits(W5500_GPIO_REG_CS, W5500_GPIO_PIN_CS);
	
	//! reset
	GPIO_InitStructure.GPIO_Pin = W5500_GPIO_PIN_RST;						 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		
	GPIO_Init(W5500_GPIO_REG_RST, &GPIO_InitStructure);							
	GPIO_SetBits(W5500_GPIO_REG_RST, W5500_GPIO_PIN_RST);						

}




/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void w5500SetMAC(void)
{	
	memcpy(DHCP_GET.mac, myMac, 6);	
	setSHAR(myMac);
    debug("w5500 mac: %02X.%02X.%02X.%02X.%02X.%02X\n", myMac[0], myMac[1], myMac[2], myMac[3], myMac[4], myMac[5]);	
}




/**
*@brief		����W5500��IP��ַ
*@param		��
*@return	��
*/
void w5500SetIP(void)
{	 
    if (gEepromLnet.dhcp == IP_STATIC) {	
		// nothing to do
        debug("get ip info from eeprom! \n");
        
	} else if (gEepromLnet.dhcp == IP_DYNAMIC) {		
		if (gFlagDHCPState == true) {
			debug("W5500: ip from dhcp server: \r\n");		 
			memcpy(gEepromLnet.ip,  DHCP_GET.lip, 4);//����DHCP��ȡ��������Ϣ�����ýṹ��
			memcpy(gEepromLnet.sub, DHCP_GET.sub, 4);
			memcpy(gEepromLnet.gay, DHCP_GET.gwy, 4);
			memcpy(gEepromLnet.dns, DHCP_GET.dns, 4);	

		} else {
			debug("get ip info from dhcp server failed! \r\n");
		}
	}
	

	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(gEepromLnet.sub);
	setGAR(gEepromLnet.gay);
	setSIPR(gEepromLnet.ip); 
	
	getSIPR(gEepromLnet.ip);
	debug("w5500 ip      : %d.%d.%d.%d\n", gEepromLnet.ip[0], gEepromLnet.ip[1], gEepromLnet.ip[2], gEepromLnet.ip[3]);
    
    getSUBR(gEepromLnet.sub);
	debug("w5500 subnet  : %d.%d.%d.%d\n", gEepromLnet.sub[0], gEepromLnet.sub[1], gEepromLnet.sub[2], gEepromLnet.sub[3]);
    
	getGAR(gEepromLnet.gay);
	debug("w5500 gateway : %d.%d.%d.%d\n", gEepromLnet.gay[0], gEepromLnet.gay[1], gEepromLnet.gay[2], gEepromLnet.gay[3]);
    debug("w5500 dns     : %d.%d.%d.%d\n", gEepromLnet.dns[0], gEepromLnet.dns[1], gEepromLnet.dns[2], gEepromLnet.dns[3]);
}



void w5500Init(void)
{
	w5500GpioConfig();
	w5500Reset();
	w5500SetMAC();

	if (gEepromLnet.dhcp == IP_STATIC) {
        w5500SetIP();
	}
	
	socket_buf_init(txsize, rxsize);
 	setRTR(2000);  
    setRCR(3);     
}




/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
*/
void w5500Reset(void)
{
	GPIO_ResetBits(W5500_GPIO_REG_RST, W5500_GPIO_PIN_RST);
	delay_ms(100);  
	GPIO_SetBits(W5500_GPIO_REG_RST, W5500_GPIO_PIN_RST);
	delay_ms(100);
}



void w5500Reboot(void)
{
	GPIO_ResetBits(W5500_GPIO_REG_CTRL, W5500_GPIO_PIN_CTRL);
	delay_ms(2000); 
	GPIO_SetBits(W5500_GPIO_REG_CTRL, W5500_GPIO_PIN_CTRL);   
}



/**
* @brief  �����ٽ���
* @retval None
*/
void IINCHIP_CRIS_ENTER(void)
{
	//__set_PRIMASK(1);
}



/**
* @brief  �˳��ٽ���
* @retval None
*/
void IINCHIP_CRIS_EXIT(void)
{
	//__set_PRIMASK(0);
}



/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8_t IINCHIP_SpiSendData(uint8_t data)
{
    uint8_t re = 0;
    IINCHIP_CRIS_ENTER();
    re = spi2ReadWriteByte(data);	
    IINCHIP_CRIS_EXIT();
    return re;
}



/**
*@brief		д��һ��8λ���ݵ�W5500
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data��д���8λ����
*@return	��
*/
void IINCHIP_WRITE(uint32_t addrbsb, uint8_t data)
{
	W5500_SELECT(); 
	IINCHIP_SpiSendData((addrbsb & 0x00FF0000)>>16);	
	IINCHIP_SpiSendData((addrbsb & 0x0000FF00)>> 8);
	IINCHIP_SpiSendData((addrbsb & 0x000000F8) + 4);  
	IINCHIP_SpiSendData(data);                   
	W5500_DESELECT();                              
}

/**
*@brief		��W5500����һ��8λ����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data����д��ĵ�ַ����ȡ����8λ����
*@return	��
*/
uint8_t IINCHIP_READ(uint32_t addrbsb)
{
	uint8_t data = 0;
	
	W5500_SELECT();                            
	IINCHIP_SpiSendData((addrbsb & 0x00FF0000)>>16);
	IINCHIP_SpiSendData((addrbsb & 0x0000FF00)>> 8);
	IINCHIP_SpiSendData((addrbsb & 0x000000F8))    ;
	data = IINCHIP_SpiSendData(0x00);            
	W5500_DESELECT();                               
	return data;    
}



/**
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
*/
uint16_t wiz_write_buf(uint32_t addrbsb, uint8_t *buf, uint16_t len)
{
	uint16_t idx = 0;
	if (len == 0) printf("Unexpected2 length 0\r\n");
	
	W5500_SELECT();                               
	IINCHIP_SpiSendData((addrbsb & 0x00FF0000)>>16);
	IINCHIP_SpiSendData((addrbsb & 0x0000FF00)>> 8);
	IINCHIP_SpiSendData((addrbsb & 0x000000F8) + 4); 
	
	for (idx = 0; idx < len; idx++) {
		IINCHIP_SpiSendData(buf[idx]);
	}
	W5500_DESELECT();  
	
	return len;  
}



/**
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
*/
uint16_t wiz_read_buf(uint32_t addrbsb, uint8_t *buf, uint16_t len)
{
	uint16_t idx = 0;
	if (len == 0) {
		printf("Unexpected2 length 0\r\n");
	}

	W5500_SELECT();                                
	IINCHIP_SpiSendData((addrbsb & 0x00FF0000)>>16);
	IINCHIP_SpiSendData((addrbsb & 0x0000FF00)>> 8);
	IINCHIP_SpiSendData((addrbsb & 0x000000F8));    
	for (idx = 0; idx < len; idx++) {
		buf[idx] = IINCHIP_SpiSendData(0x00);
	}
	W5500_DESELECT();
	
	return len;
}




/**
*@brief		ntp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void ntpTimerInit(void)
{
	timer3Init();																	
}





/**
*@brief		check if the ethernet wire exist
*@param		none
*@return	true: exist false: not exist
*/
bool w5500CheckLink(void) 
{
	if((IINCHIP_READ(PHYCFGR) & 0x01) == 0) { 
		return false;
		
	} else {
		return true;
	}
	
}


