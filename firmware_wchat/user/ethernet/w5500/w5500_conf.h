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

#define MAX_BUF_SIZE		 	1460		/*定义每个数据包的大小*/
#define TX_RX_MAX_BUF_SIZE      2048							 
					


extern eeprom_lnet_t  gEepromLnet;
extern eeprom_mqtt_t  gEepromMqtt;


extern uint32_t	ntptimer;					/*NPT秒计数*/

/*MCU配置相关函数*/
void w5500Delay(void);
void w5500GpioConfig(void);					/*SPI接口reset 及中断引脚*/																			/*定时器中断执行的函数*/
void systemReboot(void);					/*STM32软复位*/
/*W5500SPI相关函数*/
void IINCHIP_CRIS_ENTER(void);
void IINCHIP_CRIS_EXIT(void);
void 	IINCHIP_WRITE(uint32_t addrbsb, uint8_t data);				/*写入一个8位数据到W5500*/
uint8_t IINCHIP_READ(uint32_t addrbsb);									/*从W5500读出一个8位数据*/
uint16_t wiz_write_buf(uint32_t addrbsb, uint8_t *buf, uint16_t len);	/*向W5500写入len字节数据*/
uint16_t wiz_read_buf(uint32_t addrbsb, uint8_t *buf, uint16_t len);	/*从W5500读出len字节数据*/

/*W5500基本配置相关函数*/
void w5500Init(void);
void w5500Reset(void);													/*硬复位W5500*/
void w5500Reboot(void);
void w5500SetMAC(void);													/*配置W5500的MAC地址*/
void w5500SetIP(void);													/*配置W5500的IP地址*/
bool w5500CheckLink(void); 

/*需要用定时的的应用函数*/
void ntpTimerInit(void);												/*npt用到的定时器初始化*/


#endif
