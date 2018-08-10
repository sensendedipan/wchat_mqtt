


#ifndef _UTILITY_H
#define _UTILITY_H



#include <stm32f4xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "types.h"


void systick_init(uint8_t SYSCLK);
void dwtInit(void);
void delay_us(uint32_t time_us);
void delay_ms(uint32_t time_ms);


uint16_t atoi16(char* str, uint16_t base); 			/* Convert a string to integer number */
uint32_t atoi32(char* str, uint16_t base); 			/* Convert a string to integer number */
void itoa(uint16_t n, uint8_t* str, uint8_t len);
int validatoi(char* str, int base, int* ret); 		/* Verify character string and Convert it to (hexa-)decimal. */
char c2d(u_char c); 					            /* Convert a character to HEX */

uint16_t swaps(uint16_t i);
uint32_t swapl(uint32_t l);

void replacetochar(char* str, char oldchar, char newchar);

void mid(char* src, char* s1, char* s2, char* sub);
void inet_addr_(unsigned char* addr,unsigned char *ip);


char* inet_ntoa(unsigned long addr);			    /* Convert 32bit Address into Dotted Decimal Format */
char* inet_ntoa_pad(unsigned long addr);

uint32_t inet_addr(unsigned char* addr);		    /* Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address */

char verify_ip_address(char* src, uint8_t* ip);     /* Verify dotted notation IP address string */

uint16_t htons( unsigned short hostshort);	        /* htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).*/

uint32_t htonl(unsigned long hostlong);		        /* htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian). */

uint32_t ntohs(unsigned short netshort);		    /* ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors). */

uint32_t ntohl(unsigned long netlong);		        /* ntohl function converts a u_long from TCP/IP network order to host byte order (which is little-endian on Intel processors). */

uint16_t checksum(unsigned char* src, unsigned int len);		/* Calculate checksum of a stream */

uint8_t check_dest_in_local(u_long destip);			/* Check Destination in local or remote */



#endif
