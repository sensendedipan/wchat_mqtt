

#ifndef _INOUT_DRIVER_H
#define _INOUT_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "eeprom.h"
#include "data_struct.h"


//! wifi()
#define WIFI_EN_GPIO_REG 			GPIOE
#define WIFI_EN_GPIO_PIN			GPIO_Pin_10		//! PE.10

#define WIFI_RST_GPIO_REG 			GPIOE
#define WIFI_RST_GPIO_PIN			GPIO_Pin_9		//! PE.09

#define WIFI_IAP_GPIO_REG           GPIOE
#define WIFI_IAP_GPIO_PIN           GPIO_Pin_11
#define WIFI_RCC_AHB1Periph	        RCC_AHB1Periph_GPIOE


//! reboot key(digital input)
#define REBOOT_GPIO_REG 			GPIOE
#define REBOOT_GPIO_PIN			    GPIO_Pin_4		//! PE.04	
#define REBOOT_RCC_AHB1Periph	    RCC_AHB1Periph_GPIOE


//! reset key(digital input) just for parameters
#define RESET_GPIO_REG 			GPIOE
#define RESET_GPIO_PIN			GPIO_Pin_1		//! PE.01	
#define RESET_RCC_AHB1Periph	RCC_AHB1Periph_GPIOE



//! power enable switch(digital input) for simple version
#define POWER_GPIO_REG 			GPIOC
#define POWER_GPIO_PIN			GPIO_Pin_6		//! PC.06	
#define POWER_RCC_AHB1Periph	RCC_AHB1Periph_GPIOE



//! ac input detect(digital input)
#define ACINPUT_GPIO_REG 			GPIOC
#define ACINPUT_GPIO_PIN			GPIO_Pin_0	//! PC.00	
#define ACINPUT_RCC_AHB1Periph	    RCC_AHB1Periph_GPIOC



//! dc input detect(digital input)
#define DCINPUT_GPIO_REG 			GPIOC
#define DCINPUT_GPIO_PIN			GPIO_Pin_3	//! PC.03
	
#define DCINPUT_RCC_AHB1Periph	    RCC_AHB1Periph_GPIOC




//! ac output (digital output)
#define ACOUTPUT_GPIO_REG 			GPIOC
#define ACOUTPUT_GPIO_PIN			GPIO_Pin_2	//! PC.02	
#define ACOUTPUT_RCC_AHB1Periph	    RCC_AHB1Periph_GPIOC




//! kms output (digital output)
#define KM1OUTPUT_GPIO_REG 			GPIOE
#define KM1OUTPUT_GPIO_PIN			GPIO_Pin_13	//! PE.13

#define KM2OUTPUT_GPIO_REG 			GPIOE
#define KM2OUTPUT_GPIO_PIN			GPIO_Pin_12	//! PE.12

#define KM3OUTPUT_GPIO_REG 			GPIOE
#define KM3OUTPUT_GPIO_PIN			GPIO_Pin_11	//! PE.11

#define KM4OUTPUT_GPIO_REG 			GPIOE
#define KM4OUTPUT_GPIO_PIN			GPIO_Pin_10	//! PE.10

#define KMSOUTPUT_RCC_AHB1Periph    RCC_AHB1Periph_GPIOE



//! reverse
//! 功能口
#define REVERSE1_GPIO_REG           GPIOD
#define REVERSE1_GPIO_PIN           GPIO_Pin_12

#define REVERSE2_GPIO_REG           GPIOD
#define REVERSE2_GPIO_PIN           GPIO_Pin_13

//! USB口
#define REVERSE3_GPIO_REG           GPIOC
#define REVERSE3_GPIO_PIN           GPIO_Pin_6

#define REVERSE4_GPIO_REG           GPIOC
#define REVERSE4_GPIO_PIN           GPIO_Pin_7

//! 时钟
#define REVERSE6_GPIO_REG           GPIOD
#define	REVERSE6_GPIO_PIN			GPIO_Pin_2		

#define REVERSE7_GPIO_REG           GPIOD
#define	REVERSE7_GPIO_PIN			GPIO_Pin_3		

//! 片外flash
#define REVERSE8_GPIO_REG           GPIOB
#define REVERSE8_GPIO_PIN           GPIO_Pin_3

#define REVERSE9_GPIO_REG           GPIOB
#define REVERSE9_GPIO_PIN           GPIO_Pin_4

#define REVERSE10_GPIO_REG          GPIOB
#define REVERSE10_GPIO_PIN          GPIO_Pin_5

#define REVERSE11_GPIO_REG          GPIOB
#define REVERSE11_GPIO_PIN          GPIO_Pin_6

#define REVERSE_RCC_AHB1Periph      RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD



void biosInit(void);

//! wifi
void wifiConfig(void);
void wifiEnable(bool state);
void wifiReset(void);

//! reboot switch
void rebootkeyInit(void);
bool rebootkeyPressDown(void);


//! power enable switch  just for simple version
void powerkeyInit(void);
bool powerkeyPressDown(void);


//! reset key
void resetkeyInit(void);
bool resetkeyPressDown(void);


//! ac input check
void acinputCheckInit(void);
bool getACinputState(void);


//! ac input check
void dcinputCheckInit(void);
bool getDCinputState(void);


//! power output
void enableOutput(void);
void disableOutput(void);


//! kms output
void kmsOutputInit(void);
void kmsOutputEnable(uint8_t chan, bool flag);



//! reverse pins
void reverseInit(void);

#endif


