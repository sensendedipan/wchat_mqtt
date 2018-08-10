



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utility.h"
#include "w5500_conf.h"
#include "types.h"
#include "socket.h"
#include "w5500.h"
#include "dns.h"


//#define DEBUG_DNS

static uint8_t dns_ser_ips[10][4] = {
    {  1,   1,   1,   1},
    {  8,   8,   8,   8},
    {  1,   2,   4,   8},
    {  8,   8,   4,   4},
    {114, 114, 114, 114},
    {114, 114, 115, 115},
    {223,   5,   5,   5},
    {223,   6,   6,   6},
    {208,  67, 220, 220},    
    {208,  67, 222, 222}};

static uint8_t  DNS_GET_IP[4];
static uint16_t MSG_ID = 0X1122;
static uint8_t  BUFPUB[1024];
static uint8_t dns_retry_cnt = 0;
extern eeprom_lnet_t  gEepromLnet;


int dns_makequery(uint16_t op, uint8_t* name, uint8_t* buf, uint16_t len)
{
    uint8_t* cp;
    uint8_t* cp1;
    uint8_t* dname;
    uint16_t p;
    uint16_t dlen;

    cp = buf;  
    MSG_ID++;
    *(uint16_t*)&cp[0] = htons(MSG_ID);
    p = (op << 11) | 0x0100;			/* Recursion desired */
    *(uint16_t*)&cp[2] = htons(p);
    *(uint16_t*)&cp[4] = htons(1);
    *(uint16_t*)&cp[6] = htons(0);
    *(uint16_t*)&cp[8] = htons(0);
    *(uint16_t*)&cp[10]= htons(0);

    cp += sizeof(uint16_t)*6;
    dname = name;
    dlen = strlen((char*)dname);

    while(1) {
        cp1 = (unsigned char*)strchr((char*)dname, '.'); /* Look for next dot */

        if (cp1) {
        len = cp1 - dname;	                         /* More to come */

        } else {
            len = dlen;			                         /* Last component */
        }
        *cp++ = len;				                     /* Write length of component */
        if (len == 0) break;


        strncpy((char *)cp, (char*)dname, len);          /* Copy component up to (but not including) dot */
        cp += len;
        if (!cp1) {
            *cp++ = 0;			                         /* Last one; write null and finish */
            break;
        }

        dname += len+1;
        dlen -= len+1;
    }

    *(uint16_t*)&cp[0] = htons(0x0001);				    /* type */
    *(uint16_t*)&cp[2] = htons(0x0001);				    /* class */
    cp += sizeof(uint16_t)*2;

    return ((int)((uint32_t)(cp) - (uint32_t)(buf)));
}




int parse_name(uint8_t* msg, uint8_t* compressed, uint16_t len)
{
    uint16_t slen;		    /* Length of current segment */
    uint8_t* cp;
    int16_t  clen = 0;		/* Total length of compressed name */
    int16_t  indirect = 0;	/* Set if indirection encountered */
    int16_t  nseg = 0;		/* Total number of segments in name */
    int8_t   name[MAX_DNS_BUF_SIZE];
    int8_t*  buf;

    buf = name;
    cp = compressed;

    while (1) {
        slen = *cp++;	/* Length of this segment */

        if (!indirect) clen++;

        if ((slen & 0xc0) == 0xc0) {
            if (!indirect) clen++;
            indirect = 1;
            
            cp = &msg[((slen & 0x3f)<<8) + *cp];  /* Follow indirection */
            slen = *cp++;
        }

        if (slen == 0) break;	/* zero length == all done */
       
        len -= slen + 1;

        if (len <= 0) return -1;

        if (!indirect) clen += slen;

        while(slen-- != 0) *buf++ = (int8)*cp++;
        *buf++ = '.';
        nseg++;
    }

    if (nseg == 0) { /* Root name; represent as single dot */        
        *buf++ = '.';
        len--;
    }

    *buf++ = '\0';
    len--;

    return clen;	/* Length of compressed message */
}




uint8_t* dns_question(uint8_t* msg, uint8_t* cp)
{
    int16_t len;

    len = parse_name(msg, cp, MAX_DNS_BUF_SIZE);

    if (len == -1) return 0;

    cp += len;
    cp += 2;		/* type */
    cp += 2;		/* class */

    return cp;
}





uint8_t* dns_answer(uint8_t* msg, uint8_t* cp)
{
    int16_t len, type;

    len = parse_name(msg, cp, MAX_DNS_BUF_SIZE);

    if (len == -1) return 0;

    cp += len;
    type = ntohs(*((uint16_t*)&cp[0]));
    cp += 2;		/* type */
    cp += 2;		/* class */
    cp += 4;		/* ttl */
    cp += 2;		/* len */

    switch (type) {
        case TYPE_A:
            DNS_GET_IP[0] = *cp++;
            DNS_GET_IP[1] = *cp++;
            DNS_GET_IP[2] = *cp++;
            DNS_GET_IP[3] = *cp++;
            break;
        case TYPE_CNAME:
        case TYPE_MB:
        case TYPE_MG:
        case TYPE_MR:
        case TYPE_NS:
        case TYPE_PTR:
            /* These types all consist of a single domain name */
            /* convert it to ascii format */
            len = parse_name(msg, cp, /*name,*/ MAX_DNS_BUF_SIZE);
            if (len == -1) return 0;

            cp += len;
            break;
        case TYPE_HINFO:
            len = *cp++;
            cp += len;

            len = *cp++;
            cp += len;
            break;
        case TYPE_MX:
            cp += 2;
            /* Get domain name of exchanger */
            len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
            if (len == -1) return 0;

            cp += len;
            break;
        case TYPE_SOA:
            /* Get domain name of name server */
            len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
            if (len == -1) return 0;

            cp += len;

            /* Get domain name of responsible person */
            len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
            if (len == -1) return 0;

            cp += len;

            cp += 4;
            cp += 4;
            cp += 4;
            cp += 4;
            cp += 4;
            break;
        case TYPE_TXT: /* Just stash */           
            break;
        default:  /* Ignore */           
            break;
    }

    return cp;
}





uint8_t parse_msg(dhdr_t* pdhdr, uint8_t* pbuf)
{
    uint16_t tmp;
    uint16_t i;
    uint8_t * msg;
    uint8_t * cp;

    msg = pbuf;
    memset(pdhdr, 0, sizeof(dhdr_t));

    pdhdr->id = ntohs(*((uint16_t*)&msg[0]));
    tmp = ntohs(*((uint16_t*)&msg[2]));
    if (tmp & 0x8000) pdhdr->qr = 1;

    pdhdr->opcode = (tmp >> 11) & 0xf;

    if (tmp & 0x0400) pdhdr->aa = 1;
    if (tmp & 0x0200) pdhdr->tc = 1;
    if (tmp & 0x0100) pdhdr->rd = 1;
    if (tmp & 0x0080) pdhdr->ra = 1;

    pdhdr->rcode = tmp & 0xf;
    pdhdr->qdcount = ntohs(*((uint16_t*)&msg[4]));
    pdhdr->ancount = ntohs(*((uint16_t*)&msg[6]));
    pdhdr->nscount = ntohs(*((uint16_t*)&msg[8]));
    pdhdr->arcount = ntohs(*((uint16_t*)&msg[10]));

    /* Now parse the variable length sections */
    cp = &msg[12];

    /* Question section */
    for (i = 0; i < pdhdr->qdcount; i++) {
        cp = dns_question(msg, cp);
    }

    /* Answer section */
    for (i = 0; i < pdhdr->ancount; i++) {
        cp = dns_answer(msg, cp);
    }

    /* Name server (authority) section */
    for (i = 0; i < pdhdr->nscount; i++) {
        ;
    }

    /* Additional section */
    for (i = 0; i < pdhdr->arcount; i++) {
        ;
    }

    if (pdhdr->rcode == 0) {
        return 1;
        
    } else {
        return 0;
    }
}




uint8_t dns_query(uint8_t s, uint8_t* name)
{
    static uint32_t dns_wait_time = 0;
    dhdr_t dhp;
    uint8_t ip[4];
    uint16_t len, port;
    
    switch (getSn_SR(s)) {
        case SOCK_UDP:
            if ((len = getSn_RX_RSR(s)) > 0) {
                if (len > MAX_DNS_BUF_SIZE) len = MAX_DNS_BUF_SIZE;
                len = recvfrom(s, BUFPUB, len, ip, &port);
                if (parse_msg(&dhp, BUFPUB)) {
                    close(s);
                    dns_wait_time = 0;//apple
                    return DNS_RET_SUCCESS;
                } else { 
                    dns_wait_time = DNS_RESPONSE_TIMEOUT;
                }
                
            } else {
                delay_ms(1000);
                dns_wait_time++;
            }
            
            if (dns_wait_time >= DNS_RESPONSE_TIMEOUT) {
                close(s);
                dns_wait_time = 0;//apple
                return DNS_RET_FAIL;
            }
            break;
        case SOCK_CLOSED:
            dns_wait_time = 0;
            socket(s, Sn_MR_UDP, 3000, 0);
            len = dns_makequery(0, name, BUFPUB, MAX_DNS_BUF_SIZE);

            if (dns_retry_cnt == 0) {
                sendto(s, BUFPUB, len, EXTERN_DNS_SERVERIP, IPPORT_DOMAIN);                
            } else {
                sendto(s, BUFPUB, len, dns_ser_ips[dns_retry_cnt-1], IPPORT_DOMAIN);                 
            }
            break;         
    }
    return DNS_RET_PROGRESS;
}



#if 0  //! old type
bool runDNS(uint8_t* domain_name)
{
    static uint8_t dns_retry_cnt = 0;
    uint8_t dns_ok = 0;
    uint8_t domain[31] = {0};
    memcpy(domain, domain_name, 30);
    
    if (dns_ok == 1) {
        printf("dns ok \n");
        return true;
        
    } else if (dns_retry_cnt > DNS_RETRY) {
        printf("dns failed %d \n", dns_retry_cnt);
        return false;
        
    } else if (memcmp(gEepromLnet.dns, "\x00\x00\x00\x00", 4)) {  //判断DNS服务器的IP地址是否配置 
        switch (dns_query(SOCK_DNS, domain)) {                  //发出DNS请求报文和解析DNS响应报文  
            case DNS_RET_SUCCESS:								//DNS解析域名成功
                dns_ok = 1;										//DNS运行标志位置1
                dns_retry_cnt = 0;								//DNS请求报文次数置0
                memcpy(gEepromMqtt.ip, DNS_GET_IP, 4);
#ifdef DEBUG_DNS            
                printf("get remote ip by dns: %d.%d.%d.%d  %d \n", DNS_GET_IP[0], DNS_GET_IP[1], DNS_GET_IP[2], DNS_GET_IP[3], dns_retry_cnt);
#endif            
                break;
            case DNS_RET_FAIL:									//DNS解析域名失败
                dns_ok = 0;										//DNS运行标志位置0
                dns_retry_cnt++;								//DNS请求报文次数加1
#ifdef DEBUG_DNS             
                printf("Fail! Please check your network configuration or DNS server. %d \n", dns_retry_cnt);
#endif            
                break;
            default:
                break;
        }

    } else {													//如果DNS服务器IP为0.0.0.0
#ifdef DEBUG_DNS         
        printf("Invalid DNS server [%d.%d.%d.%d]\n", gEepromLnet.dns[0], gEepromLnet.dns[1], gEepromLnet.dns[2], gEepromLnet.dns[3]);
#endif
    }
}

#endif 


bool runDNS(uint8_t* domain_name)
{
    uint8_t dns_ok = 0;
    uint8_t domain[51] = {0};
    memcpy(domain, domain_name, 50);

    if (dns_ok == 1) {
        return true;
        
    } else if (dns_retry_cnt > DNS_RETRY) {
        dns_retry_cnt = 0;
        return false;
        
    } else if (memcmp(gEepromLnet.dns, "\x00\x00\x00\x00", 4)) {  //判断DNS服务器的IP地址是否配置         
        uint8_t re = dns_query(SOCK_DNS, domain);
        
        if (re == DNS_RET_SUCCESS) {
            dns_ok = 1;										//DNS运行标志位置1
            dns_retry_cnt = 0;								//DNS请求报文次数置0
            memcpy(gEepromMqtt.ip, DNS_GET_IP, 4);
#ifdef DEBUG_DNS            
            printf("get remote ip by dns: %d.%d.%d.%d \n", DNS_GET_IP[0], DNS_GET_IP[1], DNS_GET_IP[2], DNS_GET_IP[3]);
#endif      
            return true;
            
        } else if (re == DNS_RET_FAIL) {
            dns_ok = 0;										//DNS运行标志位置0
            dns_retry_cnt++;								//DNS请求报文次数加1
#ifdef DEBUG_DNS             
            printf("Fail! Please check your network configuration or DNS server. %d \n", dns_retry_cnt);
#endif               
            return false;
            
        } else {
            
        }

    } else {													//如果DNS服务器IP为0.0.0.0
#ifdef DEBUG_DNS         
        printf("Invalid DNS server [%d.%d.%d.%d]\n", gEepromLnet.dns[0], gEepromLnet.dns[1], gEepromLnet.dns[2], gEepromLnet.dns[3]);
#endif
    }
}

