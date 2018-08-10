#ifndef _W5500_CONF_H
#define _W5500_CONF_H

#include <stm32f4xx.h>
#include <stdio.h>
#include <types.h>
#include <stdbool.h>
#include "data_struct.h"
#include "at24cx_driver.h"


#define __GNUC__

typedef void (*pFunction)(void);


#define W5500_GPIO_REG_CS				GPIOB
#define	W5500_GPIO_PIN_CS				GPIO_Pin_12		//! PB.12

#define W5500_GPIO_REG_RST				GPIOE
#define	W5500_GPIO_PIN_RST				GPIO_Pin_7		//! PE.07

#define W5500_GPIO_REG_INT				GPIOD
#define	W5500_GPIO_PIN_INT				GPIO_Pin_8	    //! PD.08

#define W5500_GPIO_REG_CTRL				GPIOD
#define	W5500_GPIO_PIN_CTRL				GPIO_Pin_9		//! PD.09

#define W5500_RCC_AHB1Periph		   (RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE)



extern uint8_t	myMac[6];					/* mac */

#define MAX_BUF_SIZE		 	1460		/*����ÿ�����ݰ��Ĵ�С*/
#define TX_RX_MAX_BUF_SIZE      2048							 
					


extern eeprom_lnet_t  gEepromLnet;
extern eeprom_mqtt_t  gEepromMqtt;


extern uint32_t	ntptimer;					/*NPT�����*/

/*MCU������غ���*/
void w5500Delay(void);
void w5500GpioConfig(void);					/*SPI�ӿ�reset ���ж�����*/																			/*��ʱ���ж�ִ�еĺ���*/
void systemReboot(void);					/*STM32��λ*/
/*W5500SPI��غ���*/
void IINCHIP_CRIS_ENTER(void);
void IINCHIP_CRIS_EXIT(void);
void 	IINCHIP_WRITE(uint32_t addrbsb, uint8_t data);				/*д��һ��8λ���ݵ�W5500*/
uint8_t IINCHIP_READ(uint32_t addrbsb);									/*��W5500����һ��8λ����*/
uint16_t wiz_write_buf(uint32_t addrbsb, uint8_t *buf, uint16_t len);	/*��W5500д��len�ֽ�����*/
uint16_t wiz_read_buf(uint32_t addrbsb, uint8_t *buf, uint16_t len);	/*��W5500����len�ֽ�����*/

/*W5500����������غ���*/
void w5500Init(void);
void w5500Reset(void);													/*Ӳ��λW5500*/
void w5500Reboot(void);
void w5500SetMAC(void);													/*����W5500��MAC��ַ*/
void w5500SetIP(void);													/*����W5500��IP��ַ*/
bool w5500CheckLink(void); 

/*��Ҫ�ö�ʱ�ĵ�Ӧ�ú���*/
void ntpTimerInit(void);												/*npt�õ��Ķ�ʱ����ʼ��*/


#endif
