

#include "main.h"
#include "debug.h"
#include "alarm.h"
#include "eeprom.h"

uint8_t run_state = STATE_PHY;

void osTaskDog(void *pParameters)
{  
	static uint32_t resetKeyDelay = 0;
    static uint32_t phyOfflineCount = 0;
    const static uint32_t phyOfflineCountMax = 43200; //! about 6 hours
    const static uint32_t resetKeyTimeout = 5000;     //! means 5s
    static volatile TickType_t currentTime;
    static volatile TickType_t currentTime_feed_dog;
    static uint8_t buf_read_fifo[4096];
    
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
        if (xTaskGetTickCount() - currentTime >= 2000) {//! about 2s
            currentTime = xTaskGetTickCount();

            switch(run_state) {                               
                case STATE_PHY:
                    ledFlashSet(0, 4, 50, 150); //! 
                    break;
                
                case STATE_DHCP:
                    debug("achieve ip from dhcp server... \n");
                    ledFlashSet(0, 3, 50, 150); //! 
                    break;
                
                case STATE_DNS:
                    debug("achieve ip from dns server... \n");
                    ledFlashSet(0, 3, 50, 150); //! 
                    break;

                case STATE_CONNECT:
                    debug("try to connect to mqtt server... \n");
                    ledFlashSet(0, 2, 50, 150); //! 
                    break;

                case STATE_RUNNING:
                    //ledFlashSet(0, 1, 200, 200); //! 
                    break;
                
                dafault:
                    ledFlashSet(0, 4, 50, 150); //!
                    break;
            }                              

        } else if (xTaskGetTickCount() > 0X240C8400) {  //! 每周重启一次
            systemReboot(); 
        }

        if (xTaskGetTickCount() - currentTime_feed_dog >= 1000) {//! about 1s
            currentTime_feed_dog = xTaskGetTickCount();
            wdgFeed(); //!	feed dog 
            
            //! check w5500 phy link state at 1Hz
            gFw5500LinkState = w5500CheckLink(); 
        }            

        uint16_t rc = fifoRxGetBufDataCount();

        vTaskSuspendAll();
        if (rc > FIFO_RX_BUF_SIZE-1) debug("buf_read_fifo_rx outside: %d  \n", rc);
        
        if (rc > sizeof(buf_read_fifo)) {            
            fifoRxPopBuf(buf_read_fifo, sizeof(buf_read_fifo));
            usart1SendBuffer(buf_read_fifo, sizeof(buf_read_fifo)); 
            ledFlashSet(0, 1, 10, 10);
            
        } else if (rc > 0) { 
            fifoRxPopBuf(buf_read_fifo, rc);
            usart1SendBuffer(buf_read_fifo, rc);
            ledFlashSet(0, 1, 10, 10);  
        }
        xTaskResumeAll();
    
		vTaskDelay(1);		 
	}
	
}



