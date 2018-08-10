

#ifndef _CRC16_H
#define _CRC16_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>


uint16_t crc16(const uint8_t *buf, uint32_t len);
uint32_t crc32(const uint8_t *buf, uint32_t len);
uint8_t xorCheck(uint8_t *buffer, uint16_t len);
#endif



