

#include "main.h"


bool gFw5500LinkState = false;

eeprom_lgi_t   gEepromLgi;
eeprom_mqtt_t  gEepromMqtt;
eeprom_lnet_t  gEepromLnet;
eeprom_mlgi_t  gEepromMlgi; 

static void osTaskCreate(void)
{
	xTaskCreate(osTaskDog,  "osTaskDog", 1024, NULL, 1, NULL);    
    xTaskCreate(osTaskMqt,  "osTaskMqt", 1536, NULL, 2, NULL);
	xTaskCreate(osTaskWeb,  "osTaskWeb",  512, NULL, 2, NULL);   
}



int main(void)
{	
	__set_PRIMASK(1);
	
    
	boardInit();
	osTaskCreate();	
	vTaskStartScheduler();
    
	systemReboot(); 	//! normally it can not be running into here
	
	while(1);	
}
















































