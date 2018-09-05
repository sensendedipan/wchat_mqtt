


#include "board.h"


 

/**
  * @brief  board init
  * @retval None
  */
void boardInit(void)
{   
    dwtInit();
    iosInit();
    usart4Init();
    timer2Init();
    resetkeyInit();
    ledInit();
	i2cEepromInit();    
	spi2Init();         //! w5500   
    getDevMac(myMac);   
    eepromGetSysParameter();
    usart1Init();       //! for usbtottl    
	wdgInit();  
}

















