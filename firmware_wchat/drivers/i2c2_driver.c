


#include "i2c2_driver.h"
#include "utility.h"
#include "board.h"








static void I2C2_Delay(uint8_t us)
{
    delay_us(us);
}



/**
  * @brief  iic init
  * @retval None
  */
void i2c2Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(I2C2_RCC_AHB1Periph, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = I2C2_GPIO_PIN_SCL | I2C2_GPIO_PIN_SDA | I2C2_GPIO_PIN_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C2_GPIO, &GPIO_InitStructure);

    GPIO_SetBits(I2C2_GPIO_REG_SCL, I2C2_GPIO_PIN_SCL);
    GPIO_SetBits(I2C2_GPIO_REG_SDA, I2C2_GPIO_PIN_SDA);
    GPIO_SetBits(I2C2_GPIO_REG_CS, I2C2_GPIO_PIN_CS);
    I2C2_SDA_H();
    I2C2_SCL_H();
}




/**
* @brief  进入临界区
* @retval None
*/
static void I2C2_CRIS_ENTER(void)
{
	//__set_PRIMASK(1);
}



/**
* @brief  退出临界区
* @retval None
*/
static void I2C2_CRIS_EXIT(void)
{
	//__set_PRIMASK(0);
}


static void I2C2_SDA_IN(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Pin =  I2C2_GPIO_PIN_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C2_GPIO_REG_SDA, &GPIO_InitStructure);
}


static void I2C2_SDA_OUT(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  I2C2_GPIO_PIN_SDA;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C2_GPIO_REG_SDA, &GPIO_InitStructure);
}



void i2c2Start(void)
{
    I2C2_SDA_OUT();
	I2C2_SDA_H();
    I2C2_Delay(5);
	I2C2_SCL_H();
	I2C2_Delay(5);
	I2C2_SDA_L();
	I2C2_Delay(5);
	I2C2_SCL_L();
	I2C2_Delay(5);            
}



void i2c2Stop(void)
{
    I2C2_SDA_OUT();
	I2C2_SDA_L();
    I2C2_Delay(5);
    I2C2_SCL_L();
    I2C2_Delay(5);
	I2C2_SCL_H();
	I2C2_Delay(5);
	I2C2_SDA_H();
    I2C2_Delay(5);    
}


//ack (0:ACK 1:NAK)
static void i2c2SendACK(uint8_t ack)
{
    I2C2_SDA_OUT();
    if (ack == 0) {
        I2C2_SDA_L();
        
    } else {
        I2C2_SDA_H(); 
    }
    I2C2_SCL_H();               
    I2C2_Delay(5);              
    I2C2_SCL_L();   
    I2C2_Delay(5);  
}


//! 应答标志 0应答1没应答
static uint8_t i2c2RecvACK(void)
{
    uint8_t ack;		
    I2C2_SDA_IN();
    I2C2_SDA_H();
    I2C2_SCL_H();
    I2C2_Delay(5);
    ack = I2C2_SDA_R();   
    I2C2_SCL_L(); 
    I2C2_Delay(5);	
    return ack;
}


static bool i2c2WaitAck(void)
{
	bool re;

	I2C2_SDA_H();	
	I2C2_Delay(5);
	I2C2_SCL_H();	
	I2C2_Delay(5);
	if (I2C2_SDA_R()) {
		re = false;
        
	} else {
		re = true;
	}
	I2C2_SCL_L();
	I2C2_Delay(5);
	return re; 
}



static void i2c2Ack(void)
{
	I2C2_SDA_L();
	I2C2_Delay(5);
	I2C2_SCL_H();	
	I2C2_Delay(5);
	I2C2_SCL_L();
	I2C2_Delay(5);
	I2C2_SDA_H();	
}



static void i2c2NAck(void)
{
	I2C2_SDA_H();	
	I2C2_Delay(5);
	I2C2_SCL_H();	
	I2C2_Delay(5);
	I2C2_SCL_L();
	I2C2_Delay(5);
}



void i2c2SendByte(uint8_t byte)
{
    uint8_t Y;
    I2C2_CRIS_ENTER();
    I2C2_SDA_OUT();
    I2C2_SCL_L();
	for (uint8_t i = 0; i < 8; i++) {
        Y = (byte&0x80)>>7;
        
		if (Y == 0) {
			I2C2_SDA_L();
            
		} else {
			I2C2_SDA_H();
		}
        byte <<=1;
        
		I2C2_Delay(5);
		I2C2_SCL_H();
		I2C2_Delay(5);	
		I2C2_SCL_L();
		I2C2_Delay(10);
	}
    i2c2RecvACK();
    I2C2_CRIS_EXIT();    
}



static uint8_t i2c2ReadByte(void)
{
    uint8_t value = 0;    
    I2C2_CRIS_ENTER();	
    I2C2_Delay(5);
    I2C2_SDA_IN();
    I2C2_SDA_H();
    
	for (uint8_t i = 0; i < 8; i++) {
		value <<= 1;
		I2C2_SCL_H();
		I2C2_Delay(10);
        value = value|I2C2_SDA_R();
		I2C2_SCL_L();
		I2C2_Delay(10);
	}
    I2C2_CRIS_EXIT();    
	return value;     
}



bool i2c2WriteNByte(uint8_t addr, uint8_t *pBuf, uint8_t n)
{
    unsigned char i;
    i2c2Start();      
    i2c2SendByte(addr);

    if(!I2C2_SDA_R()) {	
        return false;
    }
    
    for (i = 0; i < n; i++) {
        i2c2SendByte(*(pBuf+i));
        if (!I2C2_SDA_R()) {
            return false;
        }
    }
    i2c2Stop();
    return true;
}


bool i2c2ReadNByte(uint8_t addr, uint8_t *pBuf, uint8_t n)
{
    i2c2Start();   
    i2c2SendByte((addr)| 0x01); 

    if (!I2C2_SDA_R()) {
        return false;
    }
    delay_us(50); 

    for (uint8_t i = 0; i < n-1; i++) {
        *(pBuf+i) = i2c2ReadByte(); 
        i2c2SendACK(0); 
    }
    *(pBuf+n-1) = i2c2ReadByte();        
    i2c2SendACK(1);
    i2c2Stop(); 
    return true;	 
}




































































