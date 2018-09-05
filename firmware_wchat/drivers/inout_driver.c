


#include "inout_driver.h"
#include "utility.h"


/****************************************************************/
//! all pins must be init at first
void iosInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);
	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_All;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_All);

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_All;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_All);
    
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_All;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_All);
    
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_All;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD, GPIO_Pin_All);
    
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_All;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOE, GPIO_Pin_All);    
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


