


#include "board.h"

#define radioDelayMs(a)		vTaskDelay(a)

#define RADIO_RX_BUF_MAX	30
uint8_t radioRxBuf[RADIO_RX_BUF_MAX];
uint8_t radioRxBufIndex = 0;
uint8_t gRadioConfigEnable = false;

/**
  * @brief  radio init
  * @note   
  * @retval None
  */
void radioInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RADIO_RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin	= RADIO_GPIOC_PINS;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, RADIO_GPIOC_PINS);
	
	GPIO_InitStructure.GPIO_Pin	= RADIO_GPIOB_PINS;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, RADIO_GPIOB_PINS);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin	= RADIO_GPIOA_PINS;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	radioSetMode(RADIO_MODE_NORMAL);
	debugLogInfo("ITSC: radio initialization OK ! \n");
}
	


/**
  * @brief  enable or disable the 433 moduler
  * @note   
  * @retval None
  */
void radioEnable(bool state)
{
	if (state) {
		GPIO_SetBits(RADIO_GPIO_REG_CTRL, RADIO_GPIO_PIN_CTRL);		
		
	} else {
		GPIO_ResetBits(RADIO_GPIO_REG_CTRL, RADIO_GPIO_PIN_CTRL);				
	}
	
}



/**
  * @brief  set or reset M0 and M1
  * @note   
  * @retval None
  */
static void radioSetM0M1(bool m0, bool m1)
{
	if (m0) {
		GPIO_SetBits(RADIO_GPIO_REG_M0, RADIO_GPIO_PIN_M0);
		
	} else {
		GPIO_ResetBits(RADIO_GPIO_REG_M0, RADIO_GPIO_PIN_M0);
	}
	
	if (m1) {
		GPIO_SetBits(RADIO_GPIO_REG_M1, RADIO_GPIO_PIN_M1);
		
	} else {
		GPIO_ResetBits(RADIO_GPIO_REG_M1, RADIO_GPIO_PIN_M1);
	}
	
}



/**
  * @brief  reset the E31-TTL-50 moduler
  * @note   should under sleep mode
  * @retval None
  */
void radioReset(void)
{
	uint8_t i;
	
	radioSetMode(RADIO_MODE_SLEEP);	
	radioDelayMs(5);
	for (i = 0; i < 3; i++) {
		usart1SendChar(0xC4);
	}
}



/**
  * @brief  get the version of the E31-TTL-50 moduler
  * @note   should under sleep mode
  * @retval None
  */
void radioGetVersion(void)
{
	uint8_t i;
	
	radioSetMode(RADIO_MODE_SLEEP);
	radioDelayMs(5);
	for (i = 0; i < 3; i++) {
		usart1SendChar(0xC3);
	}	
}



/**
  * @brief  get the parameter of the E31-TTL-50 moduler
  * @note   should under sleep mode
  * @retval None
  */
void radioGetParameter(void)
{
	uint8_t i;
	
	radioSetMode(RADIO_MODE_SLEEP);
	radioDelayMs(5);
	for (i = 0; i < 3; i++) {
		usart1SendChar(0xC1);
	}	
}



/**
  * @brief  set the parameter of the E31-TTL-50 moduler
  * @note   should under sleep mode 
  * @param  addr:
  * @param  baud:
  * @param  sped:
  * @param	chan: 0 - 255 (425MHz - 450.5MHz)
  * @retval None
  */
void radioSetParameter(uint16_t addr, uint32_t baud, uint32_t sped, uint8_t chan)
{
	uint8_t cmd[6] = {0xC0, 0xFF, 0xFF, 0x1F, 0x50, 0xC4};
	uint8_t baud_t;
	uint8_t sped_t;	

	radioSetMode(RADIO_MODE_SLEEP);
	radioDelayMs(5);

	switch (baud) 
	{
		case 1200:   baud_t = 0; break;
		case 2400:   baud_t = 1; break;
		case 4800:   baud_t = 2; break;
		case 9600:   baud_t = 3; break; //! default
		case 19200:  baud_t = 4; break;
		case 38400:  baud_t = 5; break;		
		case 57600:  baud_t = 6; break;
		case 115200: baud_t = 7; break;
		default:	 baud_t = 3; break;				
	}

	switch (sped) 
	{
		case 1200:   sped_t = 0; break; //! default
		case 2400:   sped_t = 1; break;
		case 4800:   sped_t = 2; break;
		case 9600:   sped_t = 3; break; 
		case 19200:  sped_t = 4; break;
		case 38400:  sped_t = 5; break;		
		case 50000:  sped_t = 6; break;
		case 70000:  sped_t = 7; break;
		default:	 sped_t = 0; break;				
	}	
	
	cmd[0] = 0xC0; //! always save the parameters
	cmd[1] = addr >> 8;
	cmd[2] = addr & 0x00FF;
	cmd[3] = (baud_t << 3) | (sped_t & 0x07);
	cmd[4] = chan;
	cmd[5] = 0xC4;

	usart1SendBuffer(cmd, 6); 	
}



/**
  * @brief  set the mode of the E31-TTL-50 moduler
  * @param	mode: 
  * @retval None
  */
void radioSetMode(uint8_t mode)
{
	switch (mode)
	{
		case RADIO_MODE_NORMAL:
			 radioSetM0M1(false, false);
			 break;
		case RADIO_MODE_WAKEUP:
			 radioSetM0M1(true, false);
			 break;
		case RADIO_MODE_POWER_DOWN:
			 radioSetM0M1(false, true);
			 break;
		case RADIO_MODE_SLEEP:
			 radioSetM0M1(true, true);
			 break;
		default:
			 radioSetM0M1(false, false);
			 break;
	}
	
}



















