#ifndef	__TFTP_H
#define	__TFTP_H

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

#include "utility.h"
#include "w5500_conf.h"
#include "types.h"
#include "socket.h"
#include "w5500.h"
#include "types.h"
#include "board.h"


extern bool gFrunTftp;


/* tftp message */
#define TFTP_RRQ		1
#define TFTP_WRQ		2
#define TFTP_DATA		3
#define TFTP_ACK		4
#define TFTP_ERROR		5
#define TFTP_OACK		6


/* tftp state */
#define STATE_NONE		0
#define STATE_RRQ		1
#define STATE_WRQ		2
#define STATE_DATA		3
#define STATE_ACK		4
#define STATE_OACK		5

/* tftp transfer mode */
#define TRANS_ASCII		"NETASCII"
#define TRANS_BINARY	"OCTET"

/* tftp progress state */
#define TFTP_IDLE       0
#define TFTP_START      1
#define TFTP_PROGRESS	2
#define TFTP_FAIL		3
#define TFTP_SUCCESS	4

/* define */
#define TFTP_SERVER_PORT		69
#define TFTP_TEMP_PORT			51000
#define TFTP_BLK_SIZE			512
#define MAX_MTU_SIZE			1514
#define FILE_NAME_SIZE			20


/* typedef */ 
typedef struct __tftp_data 
{
	uint16_t opcode;
	uint16_t block_num;
	uint8_t  data[1024];
} tftp_data_t;


typedef struct __tftp_error 
{
	uint16_t opcode;
	uint16_t error_code;
	uint8_t  error_msg[1024];
} tftp_error_t;


typedef struct __tftp_option 
{
	uint8_t *code;
	uint8_t *value;
} tftp_option_t;




int  runTftpClient(void);
void tftpInit(uint8_t socket, uint8_t *buf);
void tftpExit(void);
void tftpReadRequest(uint32_t server_ip, uint8_t *filename);
void tftpTimeoutHandler(void);
bool save_data(uint8_t *data, uint32_t data_len, uint16_t block_number);

#endif


