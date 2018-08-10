/**
******************************************************************************
* @file   		tftp.c
* @author  		 
* @version 		V1.0
* @date    		
* @brief   		必须使用TCP的socket才能正常工作，原因不明，且端口映射时也不能正常工作
* @brief        直接在while()中调用runTftpClient()即可， 可以用来向文件系统拷贝网页信息
* @brief        必须加任务锁
******************************************************************************
**/


#include "tftp.h"
#include "flash.h"

//#define TFTP_DEBUG 

bool flagTftpInited = false;
bool flag_firmware_updateing = false;

static int		tftp_socket = -1;
static uint8_t  tftp_filename[FILE_NAME_SIZE];
static uint32_t tftp_server_ip = 0;
static uint16_t tftp_server_port = 0;
static uint32_t tftp_state = STATE_NONE;
static uint16_t tftp_block_num = 0;
static uint32_t tftp_timeout = 5;
static uint32_t tftp_resend_flag = 0;
static uint32_t tftp_time_cnt = 0;
static uint8_t  *tftp_rx_buf = NULL;
static uint8_t  tftp_progress_state = TFTP_IDLE;
static tftp_option_t default_tftp_opt;



static void set_filename(uint8_t *file, uint32_t file_size)
{
	memcpy(tftp_filename, file, file_size);
}



static void set_server_ip(uint32_t ipaddr)
{
	tftp_server_ip = ipaddr;
}




static uint32_t get_server_ip()
{
	return tftp_server_ip;
}




static void set_server_port(uint16_t port)
{
	tftp_server_port = port;
}




static uint16_t get_server_port()
{
	return tftp_server_port;
}




static void set_tftp_state(uint32_t state)
{
	tftp_state = state;
}




static uint32_t get_tftp_state()
{
	return tftp_state;
}




static void set_tftp_timeout(uint32_t timeout)
{
	tftp_timeout = timeout;
}




static uint32_t get_tftp_timeout()
{
	return tftp_timeout;
}




static void set_block_number(uint16_t block_number)
{
	tftp_block_num = block_number;
}




static uint16_t get_block_number()
{
	return tftp_block_num;
}



static int open_tftp_socket(uint8_t sock)
{
	uint8_t sd, sck_state;
	sd = socket(sock, Sn_MR_UDP, 51000, 0);
	 
    if (sd != 1) {
#ifdef TFTP_DEBUG
		printf("[%s] socket error\r\n", __func__);
#endif
		return -1;
	}
    
	do {
		sck_state = getSn_SR(sd);
	} while (sck_state != SOCK_UDP);
    
	return sd;
}




static int send_udp_packet(int socket, uint8_t *packet, uint32_t len, uint32_t ip, uint16_t port)
{
	int snd_len;
	ip = htonl(ip);
	snd_len = sendto(socket, packet, len, (uint8_t *)&ip, port);
    
	if (snd_len != len) {
		return -1;
	}
	return snd_len;
}




static int recv_udp_packet(int socket, uint8_t *packet, uint32_t len, uint32_t *ip, uint16_t *port)
{
	uint8_t sck_state;
	int16_t recv_len;

	sck_state = getSn_SR(socket);

	if (sck_state == SOCK_UDP) {
		recv_len = getSn_RX_RSR(socket);
		if (recv_len) {
			recv_len = recvfrom(socket, packet, len, (uint8_t *)ip, port);
			if (recv_len < 0) {
#ifdef TFTP_DEBUG
				printf("[%s] recvfrom error\r\n", __func__);
#endif
				return -1;
			}
			*ip = ntohl(*ip);
			return recv_len;
		}
	}
	return -1;
}




static void close_tftp_socket(int socket)
{
	close(socket);
}




static void init_tftp(void)
{
	tftp_filename[0] = 0;

	set_server_ip(0);
	set_server_port(0);

	set_tftp_state(STATE_NONE);
	set_block_number(0);

	/* timeout flag */
	tftp_resend_flag = 0;
	tftp_time_cnt = 0;

	tftp_progress_state = TFTP_START;
}




static void tftp_cancel_timeout(void)
{
	if (tftp_resend_flag) {
		tftp_resend_flag = 0;
		tftp_time_cnt = 0;
	}
}



static void tftp_reg_timeout(void)
{
	if (tftp_resend_flag == 0) {
		tftp_resend_flag = 1;
		tftp_time_cnt = 0;
	}
}



static void process_tftp_option(uint8_t *msg, uint32_t msg_len)
{
	/* TODO Option Process */
}



static void send_tftp_ack(uint16_t block_number)
{
	uint8_t snd_buf[4];
	uint8_t *pkt = snd_buf;

	*((uint16_t *)pkt) = htons((uint16_t)TFTP_ACK);
	pkt += 2;
	*((uint16_t *)pkt) = htons(block_number);
	pkt += 2;

	send_udp_packet(tftp_socket , snd_buf, 4, get_server_ip(), get_server_port());
	tftp_reg_timeout();
#ifdef TFTP_DEBUG
	printf(">> TFTP ACK : Block Number(%d)\r\n", block_number);
#endif
}



static void send_tftp_rrq(uint8_t *filename, uint8_t *mode, tftp_option_t *opt, uint8_t opt_len)
{
	uint8_t snd_buf[MAX_MTU_SIZE];
	uint8_t *pkt = snd_buf;
	uint32_t i, len;

	*((uint16_t *)pkt) = htons(TFTP_RRQ);
	pkt += 2;
	strcpy((char *)pkt, (const char *)filename);
	pkt += strlen((char *)filename) + 1;
	strcpy((char *)pkt, (const char *)mode);
	pkt += strlen((char *)mode) + 1;

	for (i = 0 ; i < opt_len ; i++) {
		strcpy((char *)pkt, (const char *)opt[i].code);
		pkt += strlen((char *)opt[i].code) + 1;
		strcpy((char *)pkt, (const char *)opt[i].value);
		pkt += strlen((char *)opt[i].value) + 1;
	}

	len = pkt - snd_buf;

	send_udp_packet(tftp_socket,  snd_buf, len, get_server_ip(), TFTP_SERVER_PORT);
	set_tftp_state(STATE_RRQ);
	set_filename(filename, strlen((char *)filename) + 1);
	tftp_reg_timeout();
#ifdef TFTP_DEBUG
	printf(">> TFTP RRQ : FileName(%s), Mode(%s)\r\n", filename, mode);
#endif
}




static void recv_tftp_rrq(uint8_t *msg, uint32_t msg_len)
{
	/* When TFTP Server Mode */
}




static void recv_tftp_wrq(uint8_t *msg, uint32_t msg_len)
{
	/* When TFTP Server Mode */
}




static void recv_tftp_data(uint8_t *msg, uint32_t msg_len)
{
	tftp_data_t *data = (tftp_data_t *)msg;

	data->opcode = ntohs(data->opcode);
	data->block_num = ntohs(data->block_num);
#ifdef TFTP_DEBUG
	printf("<< TFTP_DATA : opcode(%d), block_num(%d)\r\n", data->opcode, data->block_num);
#endif

	switch (get_tftp_state())
	{
		case STATE_RRQ :
		case STATE_OACK :
			if (data->block_num == 1) {
				set_tftp_state(STATE_DATA);
				set_block_number(data->block_num);				
				save_data(data->data, msg_len - 4, data->block_num);
				tftp_cancel_timeout();
			}
			send_tftp_ack(data->block_num);
			break;
			
		case STATE_DATA :
			if (data->block_num == (get_block_number() + 1)) {
				set_block_number(data->block_num);
				save_data(data->data, msg_len - 4, data->block_num);								
				tftp_cancel_timeout();
			}
			send_tftp_ack(data->block_num);

			if ((msg_len - 4) < TFTP_BLK_SIZE) {
				init_tftp();				
				tftp_progress_state = TFTP_SUCCESS;               
			}
			break;
		default :
			/* invalid message */
			break;
	}
}




static void recv_tftp_ack(uint8_t *msg, uint32_t msg_len)
{
#ifdef TFTP_DEBUG
	printf("<< TFTP_ACK : \r\n");
#endif

	switch (get_tftp_state())
	{
		case STATE_WRQ :
			break;
		case STATE_ACK :
			break;
		default :
			/* invalid message */
			break;
	}
}




static void recv_tftp_oack(uint8_t *msg, uint32_t msg_len)
{
#ifdef TFTP_DEBUG
	printf("<< TFTP_OACK : \r\n");
#endif

	switch (get_tftp_state())
	{
		case STATE_RRQ :
			process_tftp_option(msg, msg_len);	
			set_tftp_state(STATE_OACK);
			tftp_cancel_timeout();
			send_tftp_ack(0);
			break;

		case STATE_WRQ :
			process_tftp_option(msg, msg_len);	
			set_tftp_state(STATE_ACK);
			tftp_cancel_timeout();
			break;

		default :
			/* invalid message */
			break;
	}
}



static void recv_tftp_error(uint8_t *msg, uint32_t msg_len)
{
	tftp_error_t *data= (tftp_error_t *)msg;

	data->opcode = ntohs(data->opcode);
	data->error_code = ntohs(data->error_code);

#ifdef TFTP_DEBUG
	printf("<< TFTP_ERROR : %d (%s)\r\n", data->error_code, data->error_msg);
	printf("[%s] Error Code : %d (%s)\r\n", __func__, data->error_code, data->error_msg);
#endif
	init_tftp();
	tftp_progress_state = TFTP_FAIL;
}



void recv_tftp_packet(uint8_t *packet, uint32_t packet_len, uint32_t from_ip, uint16_t from_port)
{
	uint16_t opcode;
	if (from_ip != get_server_ip()) {
#ifdef TFTP_DEBUG
		printf("[%s] Server IP faults\r\n", __func__);
		printf("from IP : %08X, Server IP : %08X\r\n", from_ip, get_server_ip());
#endif
        flagTftpInited = false;
        gFrunTftp = false;
		return;
	}
	opcode = ntohs(*((uint16_t *)packet));
	/* Set Server Port */
	if ((get_tftp_state() == STATE_WRQ) || (get_tftp_state() == STATE_RRQ)) {
		set_server_port(from_port);
#ifdef TFTP_DEBUG
		printf("[%s] Set Server Port : %d\r\n", __func__, from_port);
#endif
	}
    
	switch (opcode)
	{
    case TFTP_RRQ :						
        recv_tftp_rrq(packet, packet_len);
        break;
    
    case TFTP_WRQ :						
        recv_tftp_wrq(packet, packet_len);
        break;
    
    case TFTP_DATA :
        recv_tftp_data(packet, packet_len);
        break;
    
    case TFTP_ACK :
        recv_tftp_ack(packet, packet_len);
        break;
    
    case TFTP_OACK :
        recv_tftp_oack(packet, packet_len);
        break;
    case TFTP_ERROR :
        recv_tftp_error(packet, packet_len);
        break;

    default :
        // Unknown Mesage
        break;
	}
}


void tftpInit(uint8_t socket, uint8_t *buf)
{
	init_tftp();

	tftp_socket = open_tftp_socket(socket);
	tftp_rx_buf = buf;
}



void tftpExit(void)
{
	init_tftp();

	close_tftp_socket(tftp_socket);
	tftp_socket = -1;

	tftp_rx_buf = NULL;
}


int runTftpClient(void)
{
    return 0;  
}



void tftpReadRequest(uint32_t server_ip, uint8_t *filename)
{
	set_server_ip(server_ip);
#ifdef TFTP_DEBUG
	printf("[%s] Set Tftp Server : %x\r\n", __func__, server_ip);
#endif
	tftp_progress_state = TFTP_PROGRESS;
	send_tftp_rrq(filename, (uint8_t *)TRANS_BINARY, &default_tftp_opt, 1);
}



void tftpTimeoutHandler(void)
{
	if (tftp_resend_flag) tftp_time_cnt++;
}




static uint32_t FLASH_BASE_ADDR = ADDR_FLASH_TEMP;
bool save_data(uint8_t *data, uint32_t data_len, uint16_t block_number)
{ 
    return true;
}


































