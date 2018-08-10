
#include "main.h"



/*
 * vTaskSuspendAll();
 * 该调度锁内不能含有如vTaskDelayUntil() vTaskDelay() xQueueSend() 等会引起任务切换的函数
 * xTaskResumeAll(); 
*/
void osTaskWeb(void *pParameters)
{     
	while(1) 
	{         
		if (gFw5500LinkState == true) {
            vTaskSuspendAll();                 
            runWebServer();
            xTaskResumeAll();                        			
		}
         
		vTaskDelay(10);
	}
	
}









