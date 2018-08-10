


#ifndef _FLASH_H
#define _FLASH_H

#include "stm32f4xx.h" 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>                                    
#include <cstdlib>


#define ADDR_FLASH_START        ((uint32_t)0x08000000)
#define ADDR_FLASH_END          ((uint32_t)0x0807FFFF)

#define NVIC_VECT_FLASH_APP     0x10000
#define NVIC_VECT_FLASH_TEMP    0x40000


#define ADDR_FLASH_APP          ((uint32_t)0x08010000)
#define ADDR_FLASH_TEMP         ((uint32_t)0x08040000)

//! for bootloader
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */

#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */

#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */

bool getCpuId(uint32_t *mac_0, uint32_t *mac_1, uint32_t *mac_2);
bool getDevMac(uint8_t *mac);
void systemReboot(void);
void getFlashSize(void);
uint16_t flashReadHalfWord(uint32_t addr);
uint32_t flashReadWord(uint32_t addr);
void flashReadWords(uint32_t readAddr, uint32_t *pBuf, uint32_t num);
uint32_t flashGetSector(uint32_t addr);
bool flashEraseSector(uint32_t addr);
bool flashWriteWords(uint32_t addr, uint32_t *data, uint32_t num);
void flashInit(void);
void flashTest(void);
void getMqttID(uint8_t id[]);


void pvdInit(void);

#endif



