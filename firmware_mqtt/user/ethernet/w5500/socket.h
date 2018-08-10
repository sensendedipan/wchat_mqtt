
#ifndef	_SOCKET_H
#define	_SOCKET_H

#include "stm32f4xx.h"
#include "Types.h"

//! added by apple
#define SOCK_OK               1        ///< Result is OK about socket process.
#define SOCK_BUSY             0        ///< Socket is busy on processing the operation. Valid only Non-block IO Mode.
#define SOCK_FATAL            -1000    ///< Result is fatal error about socket process.


/*Socket �˿�ѡ�񣬿ɰ��Լ���ϰ�߶���*/

#define SOCK_TCPS             0
#define SOCK_HUMTEM			  0
#define SOCK_PING			  0
#define SOCK_TCPC             1
#define SOCK_TFTP             1
#define SOCK_UDPS             2
#define SOCK_WEIBO      	  2
#define SOCK_DHCP             3
#define SOCK_HTTPS            4
#define SOCK_DNS              5
#define SOCK_NETBIOS          6
#define SOCK_NTP              7


extern uint8_t socket(SOCKET s, uint8_t protocol, uint16_t port, uint8_t flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void close(SOCKET s); // Close socket
extern uint8_t connect(SOCKET s, uint8_t *addr, uint16_t port); // Establish TCP connection (Active connection)
extern void disconnect(SOCKET s); // disconnect the connection
extern uint8_t listen(SOCKET s);	// Establish TCP connection (Passive connection)
extern uint16_t send(SOCKET s, const uint8_t *buf, uint16_t len); // Send data (TCP)
extern uint16_t recv(SOCKET s, uint8_t *buf, uint16_t len);	// Receive data (TCP)
extern uint16_t sendto(SOCKET s, const uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t port); // Send data (UDP/IP RAW)
extern uint16_t recvfrom(SOCKET s, uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t *port); // Receive data (UDP/IP RAW)

#ifdef __MACRAW__
void macraw_open(void);
uint16_t macraw_send(const uint8_t *buf, uint16_t len); //Send data (MACRAW)
uint16_t macraw_recv(uint8_t *buf, uint16_t len); //Recv data (MACRAW)
#endif

#endif
/* _SOCKET_H_ */

