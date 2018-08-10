/**
******************************************************************************
* @file    netbios.c
* @author  WIZnet Software Team 
* @version V1.0
* @date    2015-02-14
* @brief   nebios ���ֽ�������������    ͨ��Ping	NETBIOS_W5500_NAME �ɻ����IP��ַ
* @attention  
******************************************************************************
*/
#include <string.h>
#include <stdio.h>

#include "w5500.h"
#include "socket.h"
#include "utility.h"
#include "netbios.h"

//#define NETBIOS_DEBUG

#define NETBIOS_W5500_NAME  "ADMIN"							    /* ����NetBIOS���� �����д*/															
#define NETBIOS_PORT        137								    /* "NetBIOS Name service"��Ĭ�϶˿� */
#define NETBIOS_NAME_LEN    16									/*NetBIOS������󳤶�*/
#define NETBIOS_MSG_MAX_LEN 512								    /*NetBIOS���ĵ���󳤶� */
#define NETBIOS_NAME_TTL    10//300000							/*NetBIOS��Ӧʱ��*/

/** NetBIOS header flags */
#define NETB_HFLAG_RESPONSE           0x8000U
#define NETB_HFLAG_OPCODE             0x7800U
#define NETB_HFLAG_OPCODE_NAME_QUERY  0x0000U
#define NETB_HFLAG_AUTHORATIVE        0x0400U
#define NETB_HFLAG_TRUNCATED          0x0200U
#define NETB_HFLAG_RECURS_DESIRED     0x0100U
#define NETB_HFLAG_RECURS_AVAILABLE   0x0080U
#define NETB_HFLAG_BROADCAST          0x0010U
#define NETB_HFLAG_REPLYCODE          0x0008U
#define NETB_HFLAG_REPLYCODE_NOERROR  0x0000U

/** NetBIOS name flags */
#define NETB_NFLAG_UNIQUE             0x8000U
#define NETB_NFLAG_NODETYPE           0x6000U
#define NETB_NFLAG_NODETYPE_HNODE     0x6000U
#define NETB_NFLAG_NODETYPE_MNODE     0x4000U
#define NETB_NFLAG_NODETYPE_PNODE     0x2000U
#define NETB_NFLAG_NODETYPE_BNODE     0x0000U


static char netbios_rx_buf[NETBIOS_MSG_MAX_LEN];
static char netbios_tx_buf[NETBIOS_MSG_MAX_LEN];


/** NetBIOS message header */
#pragma pack(1)
typedef struct _NETBIOS_HDR {
  uint16_t trans_id;
  uint16_t flags;
  uint16_t questions;
  uint16_t answerRRs;
  uint16_t authorityRRs;
  uint16_t additionalRRs;
}NETBIOS_HDR;


/** NetBIOS message name part */
typedef struct _NETBIOS_NAME_HDR {
  uint8_t  nametype;
  uint8_t  encname[(NETBIOS_NAME_LEN*2)+1];
  uint16_t type;
  uint16_t cls;
  uint32_t ttl;
  uint16_t datalen;
  uint16_t flags;
  //ip_addr_p_t addr;
  uint8_t  addr[4];
}NETBIOS_NAME_HDR;


/** NetBIOS ���Ľṹ�� */
typedef struct _NETBIOS_RESP
{
  NETBIOS_HDR      resp_hdr;
  NETBIOS_NAME_HDR resp_name;
}NETBIOS_RESP;
#pragma pack()



 /**
*@brief		  NetBIOS����
*@param		  *name_enc			����ǰ��NetBios����
*@param		  *name_dec			������NetBios����
*@param		  name_dec_len
*@return	  ��
*/
static int netbios_name_decoding(char *name_enc, char *name_dec, int name_dec_len)
{
    char *pname;
    char  cname;
    char  cnbname;
    int   index = 0;

    /* ����NetBios���� */
    pname = name_enc;
    for (;;) {       /* Every two characters of the first level-encoded name turn into one character in the decoded name. */
        cname = *pname;
        if (cname == '\0') /* no more characters */
            break;  
        
        if (cname == '.')  /* scope ID follows */
            break;    
        
        if (cname < 'A' || cname > 'Z')  /* Not legal. */
            return -1;
        
        cname -= 'A';
        cnbname = cname << 4;
        pname++;

        cname = *pname;
        if (cname == '\0' || cname == '.') { /* No more characters in the name - but we're in the middle of a pair.  Not legal. */
            return -1;
        }
        
        if (cname < 'A' || cname > 'Z') {    /* Not legal. */ 
            return -1;
        }
        
        cname -= 'A';
        cnbname |= cname;
        pname++;

        /* Do we have room to store the character? */
        if (index < NETBIOS_NAME_LEN) { /* Yes - store the character. */
            name_dec[index++] = (cnbname != ' ' ? cnbname:'\0');
        }
    }

    return 0;
}




 /**
*@brief		  ִ��NetBIOS name�������� 
*@param		  ��
*@return	  ��
*/
void runNetBios(void)
{
    uint16_t len;

    switch(getSn_SR(SOCK_NETBIOS))
    {
        case SOCK_UDP:
            if ((len = getSn_RX_RSR(SOCK_NETBIOS)) > 0) {
                unsigned char rem_ip_addr[4];
                uint16_t rem_udp_port;
                char netbios_name[NETBIOS_NAME_LEN+1];
                NETBIOS_HDR* netbios_hdr;
                NETBIOS_NAME_HDR* netbios_name_hdr;
                len = recvfrom(SOCK_NETBIOS, (unsigned char*)&netbios_rx_buf, len, rem_ip_addr, &rem_udp_port);
#ifdef NETBIOS_DEBUG
                printf("rem_ip_addr=%d.%d.%d.%d:%d\r\n", rem_ip_addr[0], rem_ip_addr[1], rem_ip_addr[2], rem_ip_addr[3], rem_udp_port);
#endif
                netbios_hdr = (NETBIOS_HDR*)netbios_rx_buf;
                netbios_name_hdr = (NETBIOS_NAME_HDR*)(netbios_hdr+1);
                
                /* ������ݰ���NetBIOS��ѯ��*/
                if (((netbios_hdr->flags & ntohs(NETB_HFLAG_OPCODE)) == ntohs(NETB_HFLAG_OPCODE_NAME_QUERY)) &&
                        ((netbios_hdr->flags & ntohs(NETB_HFLAG_RESPONSE)) == 0) &&
                         (netbios_hdr->questions == ntohs(1))) 
                {
                    /* ��NetBIOS�����н��� */
                    netbios_name_decoding((char*)(netbios_name_hdr->encname), netbios_name, sizeof(netbios_name));
#ifdef NETBIOS_DEBUG
                    printf("name is %s\r\n", netbios_name);
                    printf("defined name is %s\r\n", NETBIOS_W5500_NAME);
#endif
                    /* �������Ա�����NetBIOS��ѯ */
                    if (strcmp(netbios_name, NETBIOS_W5500_NAME) == 0) {
                        uint8_t ip_addr[4];
                        NETBIOS_RESP *resp = (NETBIOS_RESP*)netbios_tx_buf;
                        /* ����NetBIOS��Ӧ����header*/
                        resp->resp_hdr.trans_id      = netbios_hdr->trans_id;
                        resp->resp_hdr.flags         = htons(NETB_HFLAG_RESPONSE |
                                                             NETB_HFLAG_OPCODE_NAME_QUERY |
                                                             NETB_HFLAG_AUTHORATIVE |
                                                             NETB_HFLAG_RECURS_DESIRED);
                        resp->resp_hdr.questions     = 0;
                        resp->resp_hdr.answerRRs     = htons(1);
                        resp->resp_hdr.authorityRRs  = 0;
                        resp->resp_hdr.additionalRRs = 0;

                        /* ����NetBIOS��Ӧ����header����*/
                        memcpy(resp->resp_name.encname, netbios_name_hdr->encname, sizeof(netbios_name_hdr->encname));
                        resp->resp_name.nametype     = netbios_name_hdr->nametype;
                        resp->resp_name.type         = netbios_name_hdr->type;
                        resp->resp_name.cls          = netbios_name_hdr->cls;
                        resp->resp_name.ttl          = htonl(NETBIOS_NAME_TTL);
                        resp->resp_name.datalen      = htons(sizeof(resp->resp_name.flags)+sizeof(resp->resp_name.addr));
                        resp->resp_name.flags        = htons(NETB_NFLAG_NODETYPE_BNODE);
                        getSIPR(ip_addr);
                        memcpy(resp->resp_name.addr, ip_addr, 4);                        
                        /* ����NetBIOS��Ӧ */
                        sendto(SOCK_NETBIOS, (unsigned char*)resp, sizeof(NETBIOS_RESP), rem_ip_addr, rem_udp_port);
#ifdef NETBIOS_DEBUG
                        printf("send response\r\n");
#endif
                    }
                }
            }
            break;
            
        case SOCK_CLOSED:
            close(SOCK_NETBIOS);
            socket(SOCK_NETBIOS, Sn_MR_UDP, NETBIOS_PORT, 0);
            break;
        
        default:
            break;
    }
}

