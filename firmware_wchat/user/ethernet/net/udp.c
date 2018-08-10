/**
******************************************************************************
* @file   		udp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		UDP��ʾ����,��ֲʱ��Ҫ��local_ip �� remote_ip
******************************************************************************
**/
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "utility.h"
#include "udp.h"

#include "main.h"


uint8_t gUdpTxBuf[UDP_TX_BUF_SIZE] = {1,2,3,4,5,6,7,8,9}; 
uint8_t gUdpRxBuf[UDP_RX_BUF_SIZE] = {0};

static uint16_t local_port = 5000;
static uint16_t remote_port  = 6005;
static uint8_t  remote_ip[4] = {123, 132, 234, 182};

/**
*@brief		UDP���Գ���
*@param		��
*@return	��
*/
void runUDPClient(void)
{                                                              
	uint16_t len = 0;
	bool flag = false;
	static volatile TickType_t currentTime;	
    
	if (xTaskGetTickCount() - currentTime >= UDP_SEND_MS/2) { // system step rate is 500 which means each step is 2ms
		currentTime = xTaskGetTickCount();
		flag = true;		
	}
	
    switch(getSn_SR(SOCK_UDPS))									/*��ȡsocket��״̬*/
    {
    case SOCK_CLOSED:											/*socket���ڹر�״̬*/
        socket(SOCK_UDPS, Sn_MR_UDP, local_port, 0);			/*��ʼ��socket*/
      break;
    
    case SOCK_UDP:												/*socket��ʼ�����*/
        if (getSn_IR(SOCK_UDPS) & Sn_IR_RECV) {
            setSn_IR(SOCK_UDPS, Sn_IR_RECV);					/*������ж�*/
        }

        if (flag == true) {
            sendto(SOCK_UDPS, gUdpTxBuf, sizeof(gUdpTxBuf), remote_ip, remote_port); 			
        }
        
        if ((len = getSn_RX_RSR(SOCK_UDPS)) > 0) { 				/*���յ�����*/
            recvfrom(SOCK_UDPS, gUdpRxBuf, len, remote_ip, &remote_port);		/*W5500���ռ����������������*/

        }
        break;
    }
}

