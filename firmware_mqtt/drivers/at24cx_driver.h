


#ifndef _AT24CX_DRIVER_H
#define _AT24CX_DRIVER_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include "data_struct.h"
#include "inout_driver.h"

//! eeprom/AT24C128
#define AT24CX_ADDR			0xA0
#define AT24CX_PAGESIZE		64
#define AT24CX_SIZE		   (16*1024)	/* 总容量(字节) */
#define AT24CX_ADDR_BYTES	2		    /* 地址字节个数 */

#define EE_VALID            0X55
#define EE_ADDR_LGI         (10*AT24CX_PAGESIZE)
#define EE_ADDR_MQTT        (20*AT24CX_PAGESIZE)
#define EE_ADDR_LNET        (30*AT24CX_PAGESIZE)
#define EE_ADDR_MLGI        (40*AT24CX_PAGESIZE)



extern const uint8_t  EE_DEFAULT_MQTT_URL[50];
extern const uint8_t  EE_DEFAULT_MQTT_IP[4];
extern const uint16_t EE_DEFAULT_MQTT_PORT;



extern const uint8_t  EE_DEFAULT_LNET_IP[4];
extern const uint8_t  EE_DEFAULT_LNET_SUB[4];
extern const uint8_t  EE_DEFAULT_LNET_GAY[4];
extern const uint8_t  EE_DEFAULT_LNET_DNS[4];
extern const uint16_t EE_DEFAULT_LNET_PORT;



extern eeprom_lgi_t   gEepromLgi;
extern eeprom_mqtt_t  gEepromMqtt;
extern eeprom_lnet_t  gEepromLnet;
extern eeprom_mlgi_t  gEepromMlgi;

bool i2cCheckDevice(void);
void i2cEepromInit(void);
bool eepromAT24CXCheck(void);
void eepromGetSysParameter(void);
bool eepromReadBytes(uint8_t* buf, uint16_t addr, uint16_t size);
bool eepromWriteBytes(uint8_t* buf, uint16_t addr, uint16_t size);

void clear_data_from_eeprom(const uint16_t addr);

bool write_lgi_info_to_eeprom(eeprom_lgi_t data);
bool write_mqtt_info_to_eeprom(eeprom_mqtt_t data);
bool write_lnet_info_to_eeprom(eeprom_lnet_t data);
bool write_mlgi_info_to_eeprom(eeprom_mlgi_t data);

bool read_lgi_info_from_eeprom(eeprom_lgi_t *data);
bool read_mqtt_info_from_eeprom(eeprom_mqtt_t *data);
bool read_lnet_info_from_eeprom(eeprom_lnet_t *data);
bool read_mlgi_info_from_eeprom(eeprom_mlgi_t *data);

#endif


