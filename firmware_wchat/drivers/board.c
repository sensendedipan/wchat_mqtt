


#include "board.h"


 

/**
  * @brief  board init
  * @retval None
  */
void boardInit(void)
{   
    dwtInit();
    usart4Init();
    timer2Init();
    biosInit();         //! basic input/output init
    ledInit();
	i2cEepromInit();    
	spi2Init();         //! w5500   
    getDevMac(myMac);   
    eepromGetSysParameter();
    usart1Init();       //! for usbtottl    
	wdgInit();   
}

















