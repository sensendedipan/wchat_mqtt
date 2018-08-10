
#include "board.h"
#include "sim_driver.h"




/**
  * @brief  sim868 init
  * @retval None
  */
void simInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(SIM_RCC_AHB1Periph, ENABLE);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin	= SIM_GPIO_PIN_GPS_EN;
	GPIO_Init(SIM_GPIO_REG_GPS_EN, &GPIO_InitStructure);	
	GPIO_ResetBits(SIM_GPIO_REG_GPS_EN, SIM_GPIO_PIN_GPS_EN);
	
	GPIO_InitStructure.GPIO_Pin	= SIM_GPIO_PIN_PWR_EN;
	GPIO_Init(SIM_GPIO_REG_PWR_EN, &GPIO_InitStructure);	
	GPIO_SetBits(SIM_GPIO_REG_PWR_EN, SIM_GPIO_PIN_PWR_EN);	
	
	GPIO_InitStructure.GPIO_Pin	= SIM_GPIO_PIN_CTRL;
	GPIO_Init(SIM_GPIO_REG_CTRL, &GPIO_InitStructure);	
	GPIO_SetBits(SIM_GPIO_REG_CTRL, SIM_GPIO_PIN_CTRL);

	printf("huayun: sim868 initialization OK ! \n");
}
	


/**
  * @brief  sim868 reset by inner power
  * @retval None
  */
void simInnerPowerReset(void)
{
	GPIO_ResetBits(SIM_GPIO_REG_PWR_EN, SIM_GPIO_PIN_PWR_EN);
	vTaskDelay(1000);
	GPIO_SetBits(SIM_GPIO_REG_PWR_EN, SIM_GPIO_PIN_PWR_EN);
    vTaskDelay(100);
    GPIO_ResetBits(SIM_GPIO_REG_GPS_EN, SIM_GPIO_PIN_GPS_EN);
}


/**
  * @brief  sim868 reset by outer power
  * @retval None  delay must be long because the C is 470uF
  */
void simOuterPowerReset(void)
{
	GPIO_ResetBits(SIM_GPIO_REG_CTRL, SIM_GPIO_PIN_CTRL);
	vTaskDelay(2000);
	GPIO_SetBits(SIM_GPIO_REG_CTRL, SIM_GPIO_PIN_CTRL);	
    vTaskDelay(1000);
}
































