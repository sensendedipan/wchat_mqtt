
#include "main.h"



/*
 * vTaskSuspendAll();
 * �õ������ڲ��ܺ�����vTaskDelayUntil() vTaskDelay() xQueueSend() �Ȼ����������л��ĺ���
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









