

#include "wdg_driver.h"
#include "flash.h"


#define  WatchDog_Time_MS  5000



/**
  * @brief  watchdog init
  * @retval None
  */
void wdgInit(void)
{
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
		RCC_ClearFlag();
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256); //! 40000/256 = 156Hz (~6.4ms)
	
	IWDG_SetReload(WatchDog_Time_MS/6.4);  //! 500ms
	IWDG_ReloadCounter();
	IWDG_Enable();  
}



/**
  * @brief  feed dog
  * @retval None
  */
void wdgFeed(void)
{
	IWDG_ReloadCounter();
}


















