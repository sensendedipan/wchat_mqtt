

#include "board.h"
#include "debug.h"
#include "eeprom.h"
#include "i2c_driver.h"
#include "at24cx_driver.h"


#define DEBUG_EEPROM


const uint8_t  EE_DEFAULT_MQTT_URL[50] = {"ms.sdhxgk.com"};
const uint8_t  EE_DEFAULT_MQTT_IP[4] = {123, 132, 234, 182};
const uint16_t EE_DEFAULT_MQTT_PORT  = 1883;
const uint32_t EE_DEFAULT_MQTT_BAUD  = 8000000;

const uint8_t  EE_DEFAULT_LNET_IP[4] = {192, 168,   0, 253};
const uint8_t  EE_DEFAULT_LNET_SUB[4]= {255, 255, 255,   0};
const uint8_t  EE_DEFAULT_LNET_GAY[4]= {192, 168,   0,   1};
const uint8_t  EE_DEFAULT_LNET_DNS[4]= {114, 114, 114, 114};
const uint16_t EE_DEFAULT_LNET_PORT  = 5000;


bool i2cCheckDevice(void)
{   
	bool ucAck = false;

	i2cInit();	
	i2cStart();	

	i2cSendByte(AT24CX_ADDR | E2PROM_WR);
	ucAck = i2cWaitAck();
	i2cStop();	
	return ucAck;    
}



/**
  * @brief  if the eeprom check well then we concerned the eeprom init well
  * @retval none 
  */
void i2cEepromInit(void)
{   
    if(i2cCheckDevice()) {       
        printf("huayun: at24C128 initialization OK ! \n");
        
    } else {     
        printf("huayun: at24C128 initialization Failed ! \n");
    }

}



/**
  * @brief  check if the eeprom is good or bad 
  * @brief  store a flag in the last addr(32768 bytes) of the AT24C256 
  * @retval false: test failed  
  *         true:  test OK
  * @note   must be called after the system startup 
  */
bool eepromAT24CXCheck(void) 
{    
    uint8_t randnum;
    uint8_t readnum;
    eepromReadBytes(&randnum, AT24CX_SIZE-1, 1); 
    randnum++;
    eepromWriteBytes(&randnum, AT24CX_SIZE-1, 1); 
    vTaskDelay(10);
    eepromReadBytes(&readnum, AT24CX_SIZE-1, 1); 
    
    if (randnum == readnum) {
        return true;
        
    } else {
        return false;
    }   
}




bool eepromReadBytes(uint8_t* buf, uint16_t addr, uint16_t len)
{
    i2cStart();
    i2cSendByte(AT24CX_ADDR | E2PROM_WR);

    if (i2cWaitAck() != true){ 
        return false;
    }

    i2cSendByte(addr >> 8);

    if (i2cWaitAck() != true) {
        return false;
    }

    i2cSendByte(addr);

    if (i2cWaitAck() != true) { 
        return false;
    }

    i2cStart();

    i2cSendByte(AT24CX_ADDR | E2PROM_RD);

    if (i2cWaitAck() != true) {
        return false;
    }

    for (uint16_t i = 0; i < len; i++) {
        buf[i] = i2cReadByte();
        if (i != len - 1) {
            i2cAck();
            
        } else {
            i2cNAck();
        }
    }

    i2cStop();

    return true;
}




bool eepromWriteBytes(uint8_t* buf, uint16_t addr, uint16_t len)
{
    uint16_t m;
    uint16_t usAddr;
    uint16_t num = 0;
    usAddr = addr;

    if (len >= AT24CX_PAGESIZE) { 
        return false; 
    }
        
    for (uint16_t i = 0; i < len; i++) {
        if (num % 64 == 0) {
            i2cStop();
            for (m = 0; m < 1000; m++) {
                i2cStart();
                i2cSendByte(AT24CX_ADDR | E2PROM_WR);
                if (i2cWaitAck() == true) {
                    break;
                }
                
                if (m >= 1000) {
                    return false;
                }
            }

            i2cSendByte(usAddr >> 8);

            if (i2cWaitAck() != true) {
                return false;
            }

            i2cSendByte(usAddr);

            if (i2cWaitAck() != true) {
                return false;
            }
        }

        i2cSendByte(buf[i]);

        if (i2cWaitAck() != true) {
            return false;
        }
        num++;
        usAddr++;
    }

    i2cStop();
    return true;
}




void eepromGetSysParameter(void)
{         
    //! get login info
    read_lgi_info_from_eeprom(&gEepromLgi);
    delay_ms(20);
    
    if ((gEepromLgi.start != EE_VALID)||(gEepromLgi.checksum != crc16(&gEepromLgi.start, sizeof(eeprom_lgi_t)-2))) {
        gEepromLgi.start = EE_VALID;
        memcpy(gEepromLgi.oldusr, EE_DEFAULT_LGI_OLD_USR, sizeof(gEepromLgi.oldusr));
        memcpy(gEepromLgi.oldpsd, EE_DEFAULT_LGI_OLD_PSD, sizeof(gEepromLgi.oldpsd));
        memcpy(gEepromLgi.newpsd, EE_DEFAULT_LGI_NEW_PSD, sizeof(gEepromLgi.newpsd));
        memcpy(gEepromLgi.newusr, EE_DEFAULT_LGI_NEW_USR, sizeof(gEepromLgi.newusr));
        memcpy(gEepromLgi.valpsd, EE_DEFAULT_LGI_VAL_PSD, sizeof(gEepromLgi.valpsd));
        write_lgi_info_to_eeprom(gEepromLgi);
        delay_ms(20);
    }
#ifdef DEBUG_EEPROM
    printf("login: newusr:  ");
    for (uint8_t i = 0; i < sizeof(gEepromLgi.newusr); i++) {
        printf("%c", gEepromLgi.newusr[i]);
    } printf("\n");
    
    printf("login: newpsd:  ");
    for (uint8_t i = 0; i < sizeof(gEepromLgi.newpsd); i++) {
        printf("%c", gEepromLgi.newpsd[i]);
    } printf("\n");
    
    printf("login: oldusr:  ");
    for (uint8_t i = 0; i < sizeof(gEepromLgi.oldusr); i++) {
        printf("%c", gEepromLgi.oldusr[i]);
    } printf("\n");
    
    printf("login: oldpsd:  ");
    for (uint8_t i = 0; i < sizeof(gEepromLgi.oldpsd); i++) {
        printf("%c", gEepromLgi.oldpsd[i]);
    } printf("\n");
#endif
    

    //! get mqtt info
    read_mqtt_info_from_eeprom(&gEepromMqtt);
    delay_ms(20);
    if ((gEepromMqtt.start != EE_VALID)||(gEepromMqtt.checksum != crc16(&gEepromMqtt.start, sizeof(eeprom_mqtt_t)-2))||(gEepromMqtt.dns > 1)) {
        gEepromMqtt.start = EE_VALID;
        memcpy(gEepromMqtt.ip, EE_DEFAULT_MQTT_IP, sizeof(gEepromMqtt.ip));
        memset(gEepromMqtt.url,0,                  sizeof(gEepromMqtt.url));
        memcpy(gEepromMqtt.url,EE_DEFAULT_MQTT_URL,sizeof(gEepromMqtt.url));
        gEepromMqtt.port = EE_DEFAULT_MQTT_PORT;
        gEepromMqtt.dns  = DNS_DISABLE;   
        write_mqtt_info_to_eeprom(gEepromMqtt);
        delay_ms(20);        
    }
#ifdef DEBUG_EEPROM
    printf("\n\n");
    printf("mqtt: dns: %d \n", gEepromMqtt.dns);
    printf("mqtt: ip:  ");
    for (uint8_t i = 0; i < sizeof(gEepromMqtt.ip); i++) {
        printf("%d. ", gEepromMqtt.ip[i]);
    } printf("\n");

    printf("mqtt: url: ");
       printf("%s ", gEepromMqtt.url);
    printf("\n");

    printf("mqtt: port: %d \n", gEepromMqtt.port);
    
#endif

    //! get mqtt login info 
    read_mlgi_info_from_eeprom(&gEepromMlgi);
    delay_ms(20);
    if ((gEepromMlgi.start != EE_VALID)||(gEepromMlgi.checksum != crc16(&gEepromMlgi.start, sizeof(eeprom_mlgi_t)-2))) {
        gEepromMlgi.start = EE_VALID;
        memcpy(gEepromMlgi.usr,EE_DEFAULT_MQTT_USR,sizeof(gEepromMlgi.usr));
        memcpy(gEepromMlgi.psd,EE_DEFAULT_MQTT_PSD,sizeof(gEepromMlgi.psd));
        write_mlgi_info_to_eeprom(gEepromMlgi);
        delay_ms(20);        
    }  
#ifdef DEBUG_EEPROM
    printf("\n\n");
    printf("mqtt: usr:  ");
    for (uint8_t i = 0; i < sizeof(gEepromMlgi.usr); i++) {
        printf("%c", gEepromMlgi.usr[i]);
    } printf("\n");
    
    printf("mqtt: psd:  ");
    for (uint8_t i = 0; i < sizeof(gEepromMlgi.psd); i++) {
        printf("%c", gEepromMlgi.psd[i]);
    } printf("\n");     
#endif
    
    //! get lnet info
    read_lnet_info_from_eeprom(&gEepromLnet);
    delay_ms(20);
    if ((gEepromLnet.start != EE_VALID)||(gEepromLnet.checksum != crc16(&gEepromLnet.start, sizeof(eeprom_lnet_t)-2))) {
        gEepromLnet.start = EE_VALID; 
        memcpy(gEepromLnet.ip,  EE_DEFAULT_LNET_IP,  sizeof(gEepromLnet.ip));
        memcpy(gEepromLnet.sub, EE_DEFAULT_LNET_SUB, sizeof(gEepromLnet.sub));
        memcpy(gEepromLnet.gay, EE_DEFAULT_LNET_GAY, sizeof(gEepromLnet.gay));
        memcpy(gEepromLnet.dns, EE_DEFAULT_LNET_DNS, sizeof(gEepromLnet.dns));        
        gEepromLnet.port = EE_DEFAULT_LNET_PORT;
        gEepromLnet.baud = EE_DEFAULT_MQTT_BAUD;
        gEepromLnet.dhcp = IP_STATIC; //! static ip
        write_lnet_info_to_eeprom(gEepromLnet);
        delay_ms(20);        
    }
#ifdef DEBUG_EEPROM
    printf("lnet: ip:   %d.%d.%d.%d  \n", gEepromLnet.ip[0],  gEepromLnet.ip[1],  gEepromLnet.ip[2],  gEepromLnet.ip[3]);    
    printf("lnet: sub:  %d.%d.%d.%d  \n", gEepromLnet.sub[0], gEepromLnet.sub[1], gEepromLnet.sub[2], gEepromLnet.sub[3]);    
    printf("lnet: gay:  %d.%d.%d.%d  \n", gEepromLnet.gay[0], gEepromLnet.gay[1], gEepromLnet.gay[2], gEepromLnet.gay[3]);
    printf("lnet: dns:  %d.%d.%d.%d  \n", gEepromLnet.dns[0], gEepromLnet.dns[1], gEepromLnet.dns[2], gEepromLnet.dns[3]);    
#endif
        
}








bool write_lgi_info_to_eeprom(eeprom_lgi_t data)
{
    const static uint16_t addr = EE_ADDR_LGI;
    static eeprom_lgi_t temp;
    
    memcpy(&temp.start, &data.start, sizeof(eeprom_lgi_t));
    temp.start = EE_VALID;
    uint16_t checksum = crc16(&temp.start, sizeof(eeprom_lgi_t)-2);
    temp.checksum = checksum;
    
    return eepromWriteBytes(&temp.start, addr, sizeof(eeprom_lgi_t));       
}



bool read_lgi_info_from_eeprom(eeprom_lgi_t *data)
{
    const static uint16_t addr = EE_ADDR_LGI;
    return eepromReadBytes(&data->start, addr, sizeof(eeprom_lgi_t));
}



bool write_mqtt_info_to_eeprom(eeprom_mqtt_t data)
{
    const static uint16_t addr = EE_ADDR_MQTT;
    static eeprom_mqtt_t temp;

    memcpy(&temp.start, &data.start, sizeof(eeprom_mqtt_t));
    temp.start = EE_VALID; //! this means this area has been inited, when read this value != 0X55 should write a default value to init it    
    uint16_t checksum = crc16(&temp.start, sizeof(eeprom_mqtt_t)-2);
    temp.checksum = checksum;

    return eepromWriteBytes(&temp.start, addr, sizeof(eeprom_mqtt_t));        
}

 

bool read_mqtt_info_from_eeprom(eeprom_mqtt_t *data)
{
    const static uint16_t addr = EE_ADDR_MQTT;
    return eepromReadBytes(&data->start, addr, sizeof(eeprom_mqtt_t));
}



bool write_lnet_info_to_eeprom(eeprom_lnet_t data)
{
    const static uint16_t addr = EE_ADDR_LNET;
    static eeprom_lnet_t temp;

    memcpy(&temp.start, &data.start, sizeof(eeprom_lnet_t));
    temp.start = EE_VALID; //! this means this area has been inited, when read this value != 0X55 should write a default value to init it    
    uint16_t checksum = crc16(&temp.start, sizeof(eeprom_lnet_t)-2);
    temp.checksum = checksum;

    return eepromWriteBytes(&temp.start, addr, sizeof(eeprom_lnet_t));        
}

 

bool read_lnet_info_from_eeprom(eeprom_lnet_t *data)
{
    const static uint16_t addr = EE_ADDR_LNET;
    return eepromReadBytes(&data->start, addr, sizeof(eeprom_lnet_t));
}



bool write_mlgi_info_to_eeprom(eeprom_mlgi_t data)
{
    const static uint16_t addr = EE_ADDR_MLGI;
    static eeprom_mlgi_t temp;

    memcpy(&temp.start, &data.start, sizeof(eeprom_mlgi_t));
    temp.start = EE_VALID; //! this means this area has been inited, when read this value != 0X55 should write a default value to init it    
    uint16_t checksum = crc16(&temp.start, sizeof(eeprom_mlgi_t)-2);
    temp.checksum = checksum;

    return eepromWriteBytes(&temp.start, addr, sizeof(eeprom_mlgi_t));        
}

 

bool read_mlgi_info_from_eeprom(eeprom_mlgi_t *data)
{
    const static uint16_t addr = EE_ADDR_MLGI;
    return eepromReadBytes(&data->start, addr, sizeof(eeprom_mlgi_t));
}



void clear_data_from_eeprom(const uint16_t addr)
{     
    if (addr == EE_ADDR_LGI) {
        eeprom_lgi_t temp;
        memset(&temp.start, 0, sizeof(eeprom_lgi_t));
        eepromWriteBytes(&temp.start, addr, sizeof(eeprom_lgi_t));
           
    } else if (addr == EE_ADDR_MQTT) {
        eeprom_mqtt_t temp;
        memset(&temp.start, 0, sizeof(eeprom_mqtt_t));
        eepromWriteBytes(&temp.start, addr, sizeof(eeprom_mqtt_t));
        
    } else if (addr == EE_ADDR_LNET) {
        eeprom_lnet_t temp;
        memset(&temp.start, 0, sizeof(eeprom_lnet_t));
        eepromWriteBytes(&temp.start, addr, sizeof(eeprom_lnet_t));

    } else if (addr == EE_ADDR_MLGI) {
        eeprom_mlgi_t temp;
        memset(&temp.start, 0, sizeof(eeprom_mlgi_t));
        eepromWriteBytes(&temp.start, addr, sizeof(eeprom_mlgi_t));
             
    } else {
        
    }
}















