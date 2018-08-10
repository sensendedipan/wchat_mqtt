/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP 演示函数
* @attention  
************************************************************************************************
**/

#include <stm32f4xx.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tcp.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"


#include "board.h"


uint8_t gTcpRxBuf[TCP_RX_BUF_SIZE] = {0};
				                              	         

/**
  *@brief	TCP Client
  *@param	无
  *@return	无
  */
void runTCPClient(void)
{	
	uint16_t len = 0;
	bool flag = false;
	static volatile TickType_t currentTime;	
    
	if (xTaskGetTickCount() - currentTime >= TCP_SEND_MS/2) { // system step rate is 500 which means each step is 2ms
		currentTime = xTaskGetTickCount();
		flag = true;		
	}

    if (gFlagPhyLink == true) {	 
        switch(getSn_SR(SOCK_TCPC))												/*获取socket的状态*/
        {
            case SOCK_CLOSED:													/*socket处于关闭状态*/
                socket(SOCK_TCPC, Sn_MR_TCP, local_port++, Sn_MR_ND);
                gITSCMsg.payload.netData.port = local_port-1;	
                if (local_port > 10000) local_port = 5000;
                break;
            
            case SOCK_INIT:														/*socket处于初始化状态*/
                //connect(SOCK_TCPC, remote_ip, remote_port);						/*socket连接服务器*/ 
                if (connect(SOCK_TCPC, gConfigMsg.rip, ((uint16_t)(gConfigMsg.rpt[1]<<8) + gConfigMsg.rpt[0])) == 1) {
             
                } 

                break;
            
            case SOCK_ESTABLISHED:											/*socket处于连接建立状态*/
                if (getSn_IR(SOCK_TCPC) & Sn_IR_CON) {
                    setSn_IR(SOCK_TCPC, Sn_IR_CON);								/*清除接收中断标志位*/
                }	
                break;
                
            case SOCK_CLOSE_WAIT:												/*socket处于等待关闭状态*/
                disconnect(SOCK_TCPC);
                break;

            case SOCK_CLOSING:													/*socket??*/
                close(SOCK_TCPC);
                break;	
            
            default:     
                if (connect(SOCK_TCPC, gConfigMsg.rip, ((uint16_t)(gConfigMsg.rpt[1]<<8) + gConfigMsg.rpt[0])) == 1) {
 
                }  
                break;
        }
	} 	
}




bool tcpParseChar(tcp_message_t* tcpMsg, uint8_t c)
{
	bool parse_flag = false;
	static tcp_message_t rxmsg;	
	static uint16_t bufferIndex;
	static tcp_msg_parse_state_t status;
	
	switch (status)
	{
	case TCP_MSG_PARSE_STATE_UNINIT:
	case TCP_MSG_PARSE_STATE_IDLE:
		if (c == STX) {
			rxmsg.stx = c;
			status = TCP_MSG_PARSE_STATE_GOT_STX; 
		} 
		break;

	case TCP_MSG_PARSE_STATE_GOT_STX:
		rxmsg.len = c;
		status = TCP_MSG_PARSE_STATE_GOT_LENGTH; 
		break;

	case TCP_MSG_PARSE_STATE_GOT_LENGTH:
		rxmsg.msg_id = c;
		status = TCP_MSG_PARSE_STATE_GOT_MSGID;
		break;
	
	case TCP_MSG_PARSE_STATE_GOT_MSGID:
		rxmsg.payload[bufferIndex++] = c;
		if (bufferIndex == rxmsg.len) {
			status = TCP_MSG_PARSE_STATE_GOT_PAYLOAD;
		}

		break;

	case TCP_MSG_PARSE_STATE_GOT_PAYLOAD:
			rxmsg.checksum = (uint16_t)c;                
			status = TCP_MSG_PARSE_STATE_GOT_CRC1;
		break;

	case TCP_MSG_PARSE_STATE_GOT_CRC1:
			rxmsg.checksum |= (uint16_t)(c << 8);				
			status = TCP_MSG_PARSE_STATE_GOT_CRC2;		
		break;
	
	case TCP_MSG_PARSE_STATE_GOT_CRC2:
		if ((crc16(&rxmsg.payload[0], bufferIndex) == rxmsg.checksum) && (c == END)) {
			memcpy(tcpMsg, &rxmsg, sizeof(rxmsg));
			parse_flag = true;			
		}
		bufferIndex = 0;                                   
		status = TCP_MSG_PARSE_STATE_UNINIT;
        break;
	
	default:
		bufferIndex = 0;
		status = TCP_MSG_PARSE_STATE_UNINIT;

	}
	
	return parse_flag;
}
