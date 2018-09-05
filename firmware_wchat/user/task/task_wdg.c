

#include "main.h"
#include "debug.h"
#include "alarm.h"
#include "eeprom.h"

uint8_t run_state = STATE_PHY;

void osTaskDog(void *pParameters)
{  
	static uint8_t resetKeyDelay = 0;
    static uint32_t phyOfflineCount = 0;
    const static uint32_t phyOfflineCountMax = 43200; //! about 6 hours
    const static uint16_t resetKeyTimeout = 2500;  //! means 5s
    static volatile TickType_t currentTime;
    static volatile TickType_t currentTime_feed_dog;
    static uint8_t  buf_tcp_to_usart[2048]; 
  
    
	while(1)
	{      
		//! check if need set paramters back to default
		if (resetkeyPressDown()) {
			resetKeyDelay++;
			if (resetKeyDelay > resetKeyTimeout) {
                clear_data_from_eeprom(EE_ADDR_LNET);
                clear_data_from_eeprom(EE_ADDR_MQTT);
                clear_data_from_eeprom(EE_ADDR_MLGI);   
                clear_data_from_eeprom(EE_ADDR_LGI);
                               
				systemReboot();
			}

		} else {
			resetKeyDelay = 0;
		}

        //! do state indicate base time is 2s
        if (xTaskGetTickCount() - currentTime >= 1000) {//! about 2s
            currentTime = xTaskGetTickCount();

            switch(run_state) {                               
                case STATE_PHY:
                    ledFlashSet(0, 4, 50, 150); //! 
                    break;
                
                case STATE_DHCP:
                    ledFlashSet(0, 3, 50, 150); //! 
                    break;
                
                case STATE_DNS:
                    ledFlashSet(0, 3, 50, 150); //! 
                    break;

                case STATE_CONNECT:
                    ledFlashSet(0, 2, 50, 150); //! 
                    break;

                case STATE_RUNNING:
                    ledFlashSet(0, 1, 500, 500); //!                    
                    break;
                
                dafault:
                    ledFlashSet(0, 4, 50, 150); //!
                    break;
            }                              

        } else if (xTaskGetTickCount() > 0X12064200) {  //! 每周重启一次
            systemReboot(); 
        }
        
        if (xTaskGetTickCount() - currentTime_feed_dog >= 500) {//! about 1s
            currentTime_feed_dog = xTaskGetTickCount();
            wdgFeed(); //!	feed dog
            
            gFw5500LinkState = w5500CheckLink();
        }                    

        uint16_t rc = fifoRxGetBufDataCount(); 
        if (rc > sizeof(buf_tcp_to_usart)) {
            debug("fifo_tcp_to_usart: %d \n", rc);
            vTaskSuspendAll();
            fifoRxPopBuf(buf_tcp_to_usart, sizeof(buf_tcp_to_usart));
            usart1SendBuffer(buf_tcp_to_usart, sizeof(buf_tcp_to_usart));
            xTaskResumeAll();
            
        } else if (rc > 0) {
            vTaskSuspendAll();
            fifoRxPopBuf(buf_tcp_to_usart, rc);
            usart1SendBuffer(buf_tcp_to_usart, rc);
            xTaskResumeAll();
        }        
        
        
		vTaskDelay(1);		 
	}
	
}



