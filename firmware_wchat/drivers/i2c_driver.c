


#include "i2c_driver.h"






/**
  * @brief  iic init
  * @retval None
  */
void i2cInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(I2C_RCC_AHB1Periph, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = I2C_GPIO_PIN_SCL | I2C_GPIO_PIN_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);

    i2cStop();     
}



static void I2C_Delay(void)
{
    for (uint16_t i = 0; i < 30; i++);
}


/**
* @brief  进入临界区
* @retval None
*/
void I2C_CRIS_ENTER(void)
{
	//__set_PRIMASK(1);
}



/**
* @brief  退出临界区
* @retval None
*/
void I2C_CRIS_EXIT(void)
{
	//__set_PRIMASK(0);
}


static void I2C_SDA_IN(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Pin =  I2C_GPIO_PIN_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(I2C_GPIO_REG_SDA, &GPIO_InitStructure);
}


static void I2C_SDA_OUT(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  I2C_GPIO_PIN_SDA;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(I2C_GPIO_REG_SDA, &GPIO_InitStructure);
}



void i2cStart(void)
{
	I2C_SDA_H();
	I2C_SCL_H();
	I2C_Delay();
	I2C_SDA_L();
	I2C_Delay();
	I2C_SCL_L();
	I2C_Delay();   
}



void i2cStop(void)
{
	I2C_SDA_L();
	I2C_SCL_H();
	I2C_Delay();
	I2C_SDA_H();    
}


bool i2cWaitAck(void)
{
	bool re;

	I2C_SDA_H();	
	I2C_Delay();
	I2C_SCL_H();	
	I2C_Delay();
	if (I2C_SDA_R()) {
		re = false;
        
	} else {
		re = true;
	}
	I2C_SCL_L();
	I2C_Delay();
	return re; 
}



void i2cAck(void)
{
	I2C_SDA_L();
	I2C_Delay();
	I2C_SCL_H();	
	I2C_Delay();
	I2C_SCL_L();
	I2C_Delay();
	I2C_SDA_H();	
}



void i2cNAck(void)
{
	I2C_SDA_H();	
	I2C_Delay();
	I2C_SCL_H();	
	I2C_Delay();
	I2C_SCL_L();
	I2C_Delay();
}



void i2cSendByte(uint8_t byte)
{
    I2C_CRIS_ENTER();
	for (uint8_t i = 0; i < 8; i++) {		
		if (byte & 0x80) {
			I2C_SDA_H();
            
		} else {
			I2C_SDA_L();
		}
		I2C_Delay();
		I2C_SCL_H();
		I2C_Delay();	
		I2C_SCL_L();
		if (i == 7) {
			I2C_SDA_H(); 
		}
		byte <<= 1;	
		I2C_Delay();
	}
    I2C_CRIS_EXIT();    
}



uint8_t i2cReadByte(void)
{
    I2C_CRIS_ENTER();	
    uint8_t value = 0;
	for (uint8_t i = 0; i < 8; i++) {
		value <<= 1;
		I2C_SCL_H();
		I2C_Delay();
		if (I2C_SDA_R()) {
			value++;
		}
		I2C_SCL_L();
		I2C_Delay();
	}
    I2C_CRIS_EXIT();    
	return value;     
}





