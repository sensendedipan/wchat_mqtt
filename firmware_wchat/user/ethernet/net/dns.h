#ifndef	__DNS_H
#define	__DNS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "types.h"
#include "data_struct.h"


#define  DNS_RESPONSE_TIMEOUT   3     // 3 seconds
#define  DNS_RETRY              10    // 10 times
#define  DNS_RET_PROGRESS       0
#define  DNS_RET_FAIL           1
#define  DNS_RET_SUCCESS        2

#define  EXTERN_DNS_SERVERIP    gEepromLnet.dns
#define  EXTERN_DNS_ANYPORT     2000    //tsvDEVCONFnew.any_port

#define	MAX_DNS_BUF_SIZE	    256		/* maximum size of DNS buffer. */


#define  INITRTT        2000L	            /* Initial smoothed response time */
#define  MAXCNAME       10	                /* Maximum amount of cname recursion */
         
#define  TYPE_A		    1	/* Host address */
#define  TYPE_NS		2	/* Name server */
#define  TYPE_MD		3	/* Mail destination (obsolete) */
#define  TYPE_MF		4	/* Mail forwarder (obsolete) */
#define  TYPE_CNAME	    5	/* Canonical name */
#define  TYPE_SOA       6	/* Start of Authority */
#define  TYPE_MB		7	/* Mailbox name (experimental) */
#define  TYPE_MG		8	/* Mail group member (experimental) */
#define  TYPE_MR		9	/* Mail rename name (experimental) */
#define  TYPE_NULL	    10	/* Null (experimental) */
#define  TYPE_WKS       11	/* Well-known sockets */
#define  TYPE_PTR       12	/* Pointer record */
#define  TYPE_HINFO	    13	/* Host information */
#define  TYPE_MINFO	    14	/* Mailbox information (experimental)*/
#define  TYPE_MX		15	/* Mail exchanger */
#define  TYPE_TXT       16	/* Text strings */
#define  TYPE_ANY       255 /* Matches any type */
         
#define  CLASS_IN       1   /* The ARPA Internet */

/* Round trip timing parameters */
#define  AGAIN          8	/* Average RTT gain = 1/8 */
#define  LAGAIN         3	/* Log2(AGAIN) */
#define  DGAIN          4   /* Mean deviation gain = 1/4 */
#define  LDGAIN         2   /* log2(DGAIN) */
         
#define  IPPORT_DOMAIN	53

/* Header for all domain messages */
typedef __packed struct __dhdr_t
{
	uint16_t  id;		/* Identification */
	uint8_t   qr;		/* Query/Response */
#define	QUERY		0
#define	RESPONSE	1
	uint8_t   opcode;
#define	IQUERY		1
	uint8_t 	aa;		/* Authoratative answer */
	uint8_t 	tc;		/* Truncation */
	uint8_t 	rd;		/* Recursion desired */
	uint8_t 	ra;		/* Recursion available */
	uint8_t 	rcode;		/* Response code */
#define	NO_ERROR	    0
#define	FORMAT_ERROR	1
#define	SERVER_FAIL	    2
#define	NAME_ERROR	    3
#define	NOT_IMPL	    4
#define	REFUSED		    5
	uint16_t  qdcount;	/* Question count */
	uint16_t  ancount;	/* Answer count */
	uint16_t  nscount;	/* Authority (name server) count */
	uint16_t  arcount;	/* Additional record count */
} dhdr_t;


uint8_t dns_query(uint8_t s, uint8_t* name);
bool runDNS(uint8_t* domain_name);


#endif	
