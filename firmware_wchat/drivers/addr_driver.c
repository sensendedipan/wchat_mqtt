


#include "board.h"



/**
  * @brief  addr init
  * @note   
  */
void addrInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADDR_RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIO_PIN_0;
	GPIO_Init(ADDR_GPIO_REG_0, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIO_PIN_1;
	GPIO_Init(ADDR_GPIO_REG_1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIO_PIN_2;
	GPIO_Init(ADDR_GPIO_REG_2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIO_PIN_3;
	GPIO_Init(ADDR_GPIO_REG_3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIO_PIN_4;
	GPIO_Init(ADDR_GPIO_REG_4, &GPIO_InitStructure);
	
	debugLogInfo("ITSC: addr initialization OK ! \n");
}


/**
  * @brief  get the switch position code
  * @note
  * @return 0 ~ 31
  */
uint8_t readAddr(void) 
{
	uint8_t addr = 0;
	
	addr = 	GPIO_ReadInputDataBit(ADDR_GPIO_REG_0, ADDR_GPIO_PIN_0) << 0 |
			GPIO_ReadInputDataBit(ADDR_GPIO_REG_1, ADDR_GPIO_PIN_1) << 1 |
			GPIO_ReadInputDataBit(ADDR_GPIO_REG_2, ADDR_GPIO_PIN_2) << 2 |
			GPIO_ReadInputDataBit(ADDR_GPIO_REG_3, ADDR_GPIO_PIN_3) << 3 |
			GPIO_ReadInputDataBit(ADDR_GPIO_REG_4, ADDR_GPIO_PIN_4) << 4 ;
	
	return addr;
}











/****************************************************************/
//! test key(digital input)

/**
  * @brief  test key init
  * @note   get the key state. when press down the test key, the system will run self check 
  */
void testkeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TEST_RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin	= TEST_GPIO_PIN;
	GPIO_Init(TEST_GPIO_REG, &GPIO_InitStructure);	
	
	debugLogInfo("ITSC: testkey initialization OK ! \n");
}



/**
  * @brief  check testkey state
  * @note   get the key state. return true when press down the testkey
  */
bool testkeyPressDown(void)
{
	return (GPIO_ReadInputDataBit(TEST_GPIO_REG, TEST_GPIO_PIN) == 0) ? true : false;	
}













/****************************************************************/
//! power test(digital input)

/**
  * @brief  power check init
  * @note   get the power state
  */
void powerCheckInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(POWER_RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin	= POWER_GPIO_PIN;
	GPIO_Init(POWER_GPIO_REG, &GPIO_InitStructure);
	
	gITSCMsg.payload.power_state = (getPowerState() == false) ? 1:2;
	debugLogInfo("ITSC: power check initialization OK ! \n");	
}



/**
  * @brief  check power state
  * @note   get the power state. return true when power shutdown unexpected
  */
bool getPowerState(void)
{
	return (GPIO_ReadInputDataBit(POWER_GPIO_REG, POWER_GPIO_PIN) == 0) ? true : false;	
}










/****************************************************************/
//! fan control(digital output)

/**
  * @brief  enable fan when temperature is too high
  * @note   
  */
void fanCtrlInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(FAN_RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin	= FAN_GPIO_PIN;
	GPIO_Init(FAN_GPIO_REG, &GPIO_InitStructure);	
	
	GPIO_ResetBits(FAN_GPIO_REG, FAN_GPIO_PIN);
	
	debugLogInfo("ITSC: fan initialization OK ! \n");	
}



/**
  * @brief  enable or disable the fan due to the temperature
  * @note   
  */
void fanEnable(bool state)
{
	if (state) {
		GPIO_SetBits(FAN_GPIO_REG, FAN_GPIO_PIN);		
		
	} else {
		GPIO_ResetBits(FAN_GPIO_REG, FAN_GPIO_PIN);			
	}
}








