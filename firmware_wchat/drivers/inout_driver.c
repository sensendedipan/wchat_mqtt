


#include "inout_driver.h"
#include "utility.h"

/****************************************************************/
//! reboot key(digital input) for standrad version

/**
  * @brief  reboot key init
  * @note   get the key state. when press down the reset key, the system will reboot 
  */
void rebootkeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RESET_RCC_AHB1Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin	= REBOOT_GPIO_PIN;
	GPIO_Init(REBOOT_GPIO_REG, &GPIO_InitStructure);	
	
	printf("huayun: rebootkey initialization OK ! \n");
}



/**
  * @brief  check sys key state
  * @note   return true when press down the reset, if pressed more than 5s, sysreboot
  */
bool rebootkeyPressDown(void)
{
	return (GPIO_ReadInputDataBit(REBOOT_GPIO_REG, REBOOT_GPIO_PIN) == 0) ? true : false;	
}






/****************************************************************/
//! reset key(digital input)

/**
  * @brief  reset key init
  * @note   get the key state. when press down the reset key, the system will reset the parameters 
  */
void resetkeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RESET_RCC_AHB1Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin	= RESET_GPIO_PIN;
	GPIO_Init(RESET_GPIO_REG, &GPIO_InitStructure);	
	
	printf("huayun: resetkey initialization OK ! \n");
}



/**
  * @brief  check resetkey state
  * @note   return true when press down the reset, if pressed more than 5s, reset the parameters to default
  */
bool resetkeyPressDown(void)
{
	return (GPIO_ReadInputDataBit(RESET_GPIO_REG, RESET_GPIO_PIN) == 0) ? true : false;	
}










/****************************************************************/
//! power enable switch(digital input)

/**
  * @brief  power enable switch init
  * @note   get the switch state. when press down the power key, the system will enable power output
  */
void powerkeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(POWER_RCC_AHB1Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin	= POWER_GPIO_PIN;
	GPIO_Init(POWER_GPIO_REG, &GPIO_InitStructure);	
	
	printf("huayun: powerKey initialization OK ! \n");
}



/**
  * @brief  
  * @note   
  */
bool powerkeyPressDown(void)
{
	return (GPIO_ReadInputDataBit(POWER_GPIO_REG, POWER_GPIO_PIN) == 0) ? true : false;	
}









/****************************************************************/
//! 220ac power input check(digital input)

/**
  * @brief  ac220 input check init
  * @note   true means get 220ac input, false means no ac220 input
  */
void acinputCheckInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(ACINPUT_RCC_AHB1Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin	= ACINPUT_GPIO_PIN;
	GPIO_Init(ACINPUT_GPIO_REG, &GPIO_InitStructure);	
	
	printf("huayun: acinput check initialization OK ! \n");        
}


bool getACinputState(void)
{
    return (GPIO_ReadInputDataBit(ACINPUT_GPIO_REG, ACINPUT_GPIO_PIN) == 0) ? true : false;	   
}








/****************************************************************/
//! dc power input check(digital input)

/**
  * @brief  dc12 input check init
  * @note   true means get dc12 input, false means no dc12 input
  */
void dcinputCheckInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DCINPUT_RCC_AHB1Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin	= DCINPUT_GPIO_PIN;
	GPIO_Init(DCINPUT_GPIO_REG, &GPIO_InitStructure);	
	
	printf("huayun: dcinput check initialization OK ! \n");        
}


bool getDCinputState(void)
{
    return (GPIO_ReadInputDataBit(DCINPUT_GPIO_REG, DCINPUT_GPIO_PIN) == 1) ? true : false;	   
}










/****************************************************************/
//! power output control(digital output)

/**
  * @brief   
  * @note   when press down the power key, the system will enable power output
  */
void powerOutputInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(ACOUTPUT_RCC_AHB1Periph, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin	= ACOUTPUT_GPIO_PIN;
	GPIO_Init(ACOUTPUT_GPIO_REG, &GPIO_InitStructure);		
	GPIO_SetBits(ACOUTPUT_GPIO_REG, ACOUTPUT_GPIO_PIN);   
	printf("huayun: power output initialization OK ! \n");
}



void enableOutput(void)
{
    GPIO_ResetBits(ACOUTPUT_GPIO_REG, ACOUTPUT_GPIO_PIN);	
}



void disableOutput(void)
{
    GPIO_SetBits(ACOUTPUT_GPIO_REG, ACOUTPUT_GPIO_PIN);
}




/****************************************************************/
//! kmx output control(digital output)

/**
  * @brief   
  * @note   
  */
void kmsOutputInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(KMSOUTPUT_RCC_AHB1Periph, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin	= KM4OUTPUT_GPIO_PIN;
	GPIO_Init(KM4OUTPUT_GPIO_REG, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin	= KM3OUTPUT_GPIO_PIN;
	GPIO_Init(KM3OUTPUT_GPIO_REG, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin	= KM2OUTPUT_GPIO_PIN;
	GPIO_Init(KM2OUTPUT_GPIO_REG, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin	= KM1OUTPUT_GPIO_PIN;
	GPIO_Init(KM1OUTPUT_GPIO_REG, &GPIO_InitStructure);

    
	GPIO_SetBits(KM1OUTPUT_GPIO_REG, KM1OUTPUT_GPIO_PIN); 
    GPIO_SetBits(KM2OUTPUT_GPIO_REG, KM2OUTPUT_GPIO_PIN);
    GPIO_SetBits(KM3OUTPUT_GPIO_REG, KM3OUTPUT_GPIO_PIN);
    GPIO_SetBits(KM4OUTPUT_GPIO_REG, KM4OUTPUT_GPIO_PIN);
    
	printf("huayun: kms output initialization OK ! \n");
}




void kmsOutputEnable(uint8_t chan, bool flag)
{
    switch(chan) {
        case KM_CHAN_KM1:
            if (flag) {
                GPIO_ResetBits(KM1OUTPUT_GPIO_REG, KM1OUTPUT_GPIO_PIN);                
            } else {
                GPIO_SetBits(KM1OUTPUT_GPIO_REG, KM1OUTPUT_GPIO_PIN);                
            }
            break;
        
        case KM_CHAN_KM2:
            if (flag) {
                GPIO_ResetBits(KM2OUTPUT_GPIO_REG, KM2OUTPUT_GPIO_PIN);                
            } else {
                GPIO_SetBits(KM2OUTPUT_GPIO_REG, KM2OUTPUT_GPIO_PIN);                
            }
            break;
        
        case KM_CHAN_KM3:
            if (flag) {
                GPIO_ResetBits(KM3OUTPUT_GPIO_REG, KM3OUTPUT_GPIO_PIN);                
            } else {
                GPIO_SetBits(KM3OUTPUT_GPIO_REG, KM3OUTPUT_GPIO_PIN);                
            }
            break;
        
        case KM_CHAN_KM4:
            if (flag) {
                GPIO_ResetBits(KM4OUTPUT_GPIO_REG, KM4OUTPUT_GPIO_PIN);                
            } else {
                GPIO_SetBits(KM4OUTPUT_GPIO_REG, KM4OUTPUT_GPIO_PIN);                
            }
            break;
        
        default:
            break;
    }
    	
}




void reverseInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(REVERSE_RCC_AHB1Periph, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin	= REVERSE1_GPIO_PIN;
	GPIO_Init(REVERSE1_GPIO_REG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin	= REVERSE2_GPIO_PIN;
	GPIO_Init(REVERSE2_GPIO_REG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin	= REVERSE3_GPIO_PIN;
	GPIO_Init(REVERSE3_GPIO_REG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin	= REVERSE4_GPIO_PIN;
	GPIO_Init(REVERSE4_GPIO_REG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin	= REVERSE6_GPIO_PIN;
	GPIO_Init(REVERSE6_GPIO_REG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin	= REVERSE7_GPIO_PIN;
	GPIO_Init(REVERSE7_GPIO_REG, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin	= REVERSE8_GPIO_PIN;
	GPIO_Init(REVERSE8_GPIO_REG, &GPIO_InitStructure);
        
	GPIO_InitStructure.GPIO_Pin	= REVERSE9_GPIO_PIN;
	GPIO_Init(REVERSE9_GPIO_REG, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin	= REVERSE10_GPIO_PIN;
	GPIO_Init(REVERSE10_GPIO_REG, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin	= REVERSE11_GPIO_PIN;
	GPIO_Init(REVERSE11_GPIO_REG, &GPIO_InitStructure);    

    
	GPIO_SetBits(REVERSE1_GPIO_REG, REVERSE1_GPIO_PIN);
	GPIO_SetBits(REVERSE2_GPIO_REG, REVERSE2_GPIO_PIN);
    GPIO_SetBits(REVERSE3_GPIO_REG, REVERSE3_GPIO_PIN);
    GPIO_SetBits(REVERSE4_GPIO_REG, REVERSE4_GPIO_PIN);
    GPIO_SetBits(REVERSE6_GPIO_REG, REVERSE6_GPIO_PIN);
    GPIO_SetBits(REVERSE7_GPIO_REG, REVERSE7_GPIO_PIN);
    GPIO_SetBits(REVERSE8_GPIO_REG, REVERSE8_GPIO_PIN);
    GPIO_SetBits(REVERSE9_GPIO_REG, REVERSE9_GPIO_PIN);
    GPIO_SetBits(REVERSE10_GPIO_REG, REVERSE10_GPIO_PIN);
    GPIO_SetBits(REVERSE11_GPIO_REG, REVERSE11_GPIO_PIN);  
  
}



/****************************************************************/
//! wifi control(digital output)

/**
  * @brief   
  * @note   
  */
void wifiConfig(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(WIFI_RCC_AHB1Periph, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin	= WIFI_EN_GPIO_PIN;
	GPIO_Init(WIFI_EN_GPIO_REG, &GPIO_InitStructure);		
	GPIO_SetBits(WIFI_EN_GPIO_REG, WIFI_EN_GPIO_PIN);   

    GPIO_InitStructure.GPIO_Pin	= WIFI_RST_GPIO_PIN;
	GPIO_Init(WIFI_RST_GPIO_REG, &GPIO_InitStructure);		
	GPIO_SetBits(WIFI_RST_GPIO_REG, WIFI_RST_GPIO_PIN);
    
    GPIO_InitStructure.GPIO_Pin	= WIFI_IAP_GPIO_PIN;
	GPIO_Init(WIFI_IAP_GPIO_REG, &GPIO_InitStructure);		
	GPIO_SetBits(WIFI_IAP_GPIO_REG, WIFI_IAP_GPIO_PIN);
}



void wifiEnable(bool state)
{
    if (state) {
        GPIO_SetBits(WIFI_EN_GPIO_REG, WIFI_EN_GPIO_PIN);
        
    } else {
        GPIO_ResetBits(WIFI_EN_GPIO_REG, WIFI_EN_GPIO_PIN);
    }
}



void wifiReset(void)
{
    GPIO_ResetBits(WIFI_RST_GPIO_REG, WIFI_RST_GPIO_PIN);
    delay_ms(100);
    GPIO_SetBits(WIFI_RST_GPIO_REG, WIFI_RST_GPIO_PIN);
}

void biosInit(void)
{
    resetkeyInit();     
}

