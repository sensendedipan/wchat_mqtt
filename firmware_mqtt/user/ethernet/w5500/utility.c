/**
******************************************************************************
* @file    			utility.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief 				����ʵ�ú��� ��ʱ���� ���ݸ�ʽת�� 
******************************************************************************
*/


#include "utility.h"
#include "stdlib.h"


#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR   *(volatile unsigned int *)0xE0042004

#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)



static uint8_t  fac_us = 0;						/*us��ʱ������*/
static uint16_t fac_ms = 0;						/*ms��ʱ������*/




void dwtInit(void)
{
	DEM_CR         |= (unsigned int)DEM_CR_TRCENA;   /* Enable Cortex-M4's DWT CYCCNT reg.  */
	DWT_CYCCNT      = (unsigned int)0u;
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}






#if 1
void delay_us(uint32_t time_us)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = DWT_CYCCNT;                                     /* �ս���ʱ�ļ�����ֵ */
	tCnt = 0;
	tDelayCnt = time_us * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
	}
}
#endif


/***************************************
*	�� �� ��: bsp_DelayMS
*	����˵��: Ϊ���õײ������ڴ�RTOS�����������и��õļ�����
*             ר������һ������ʽ���ӳٺ������ڵײ�������ms�����ӳ���Ҫ���ڳ�ʼ����������Ӱ��ʵʱ�ԡ�
*	��    ��: n �ӳٳ��ȣ���λ1 ms
*	�� �� ֵ: ��
****************************************/
#if 1
void delay_ms(uint32_t time_ms)
{
	delay_us(1000*time_ms);
}
#endif


/**
*@brief		��ʼ���ӳٺ���
*@param		SYSCLK:ϵͳʱ��
*@return	��
*/
void systick_init(uint8_t sysclk)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = sysclk/8;
	fac_ms = (uint16_t)fac_us*999;       
}								    




/**
*@brief	    ������ʱ����
*@param		time_ms:Ҫ��ʱ����ʱ����
*@return	��
*/
#if 0
void delay_ms(uint32_t time_ms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD = (uint32_t)time_ms*fac_ms;
	SysTick->VAL = 0x00;          
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;        
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16))); 
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;     
	SysTick->VAL = 0X00;  	    
}   

#endif



/**
*@brief	    ΢����ʱ����
*@param		time_ms:Ҫ��ʱ΢��ʱ����
*@return	��
*/

#if 0
void delay_us(uint32_t time_us)
{		
	uint32_t temp;	    	 
	SysTick->LOAD = time_us*fac_us; 	  		 
	SysTick->VAL = 0x00; 
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do
	{
		temp = SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}

#endif



/**
*@brief	 	�ַ�תת��Ϊ8λ���ͺ���
*@param		str:Ҫת���ַ����� base:
*@return	num:����ת�����������
*/
uint16_t atoi16(char* str, uint16_t base)
{
    unsigned int num = 0;
    while (*str != 0)
        num = num * base + c2d(*str++);
    return num;
}




/**
*@brief	 	�ַ�תת��Ϊ32λ���ͺ���
*@param		str:Ҫת���ַ����� base:
*@return	num:����ת�����������
*/
uint32_t atoi32(char* str, uint16_t base)
{
    uint32_t num = 0;
    while (*str !=0)
        num = num * base + c2d(*str++);
    return num;
}




/**
*@brief	 	������ת��Ϊ�ַ�������
*@param		n:Ҫת�������� str[5]:���ת������ַ���  len������������
*@return	��
*/
void itoa(uint16_t n, uint8_t str[5], uint8_t len)
{
  
    uint8_t i = len-1;

    memset(str, 0x20, len);
    
    do {
        str[i--] = n%10+'0';

    } while ((n/=10)>0);

    return;
}




/**
*@brief	 	���ַ���ת��Ϊʮ���ƻ�ʮ������������
*@param		str:Ҫת���ַ����� len������������
*@return	�ɹ� - 1, ʧ�� - 0
*/
int validatoi(char* str, int base, int* ret)
{
    int c;
    char* tstr = str;
    
    if (str == 0 || *str == '\0') return 0;
    while (*tstr != '\0') {
        c = c2d(*tstr);
        if (c >= 0 && c < base) tstr++;
        else    return 0;
    }

    *ret = atoi16(str, base);
    return 1;
}




/**
*@brief	 	���µ��ַ�ȥ�滻�ַ�����������ַ�
*@param		str:�滻���ַ�����oldchar:������ַ���newchar���µ��ַ�	
*@return	��
*/
void replacetochar(char* str, char oldchar, char newchar)
{
    int x;
    for (x = 0; str[x]; x++) 
        if (str[x] == oldchar) str[x] = newchar;	
}




/**
*@brief	 	��ʮ������ת��Ϊ�ַ���
*@param		c:Ҫת��ʮ��������
*@return	����һ���ַ�������
*/
char c2d(uint8_t c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c -'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c -'A';

	return (char)c;
}




/**
*@brief	 	16λ�ַ���8λ��8λת��
*@param		i:Ҫת��������
*@return	ת���������
*/
uint16_t swaps(uint16_t i)
{
    uint16_t ret = 0;
    ret = (i & 0xFF) << 8;
    ret |= ((i >> 8)& 0xFF);
    return ret;	
}




/**
*@brief	 	32λ�ַ��ߵ�λ�任
*@param		i:Ҫת��������
*@return	ת���������
*/
uint32_t swapl(uint32_t l)
{
    uint32_t ret = 0;
    ret = (l & 0xFF) << 24;
    ret |= ((l >> 8) & 0xFF) << 16;
    ret |= ((l >> 16) & 0xFF) << 8;
    ret |= ((l >> 24) & 0xFF);
    return ret;
}





/**
*@brief	 	�ַ�������
*@param		srcĿ���ַ��� s1 s2�����ַ���
*@return	��
*/
void mid(char* src, char* s1, char* s2, char* sub)
{
	char* sub1 = NULL;
	char* sub2 = NULL;
	uint16_t n;

    sub1 = strstr(src, s1);
    sub1 += strlen(s1);
    sub2 = strstr(sub1, s2);
    n = sub2-sub1;
    strncpy(sub, sub1, n);
    sub[n] = 0;
}




/**
*@brief	 	ip�����ַת��
*@param		adr����ַ ip��ip
*@return	��
*/
void inet_addr_(unsigned char* addr, unsigned char* ip)
{
	int i;
	char taddr[16];
	char *nexttok;
	uint16_t num;
    uint16_t temp_ip[4] = {0}; //! ����16λ��Ϊ�˼����Ƿ���� ��255��
    unsigned char num_point = 0;
    
	strcpy(taddr, (char *)addr);
    for (uint8_t i = 0; i < 16; i++) {
        if (taddr[i] == '.') num_point++;
    }
    if (num_point != 3) return; //! '.'�ĸ���û��������˵�������ip�Ƿ�
                                      
	nexttok = taddr;
	for (i = 0; i < 4 ; i++) {
		nexttok = strtok(nexttok, ".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = atoi16(nexttok+2, 0x10);
		else num = atoi16(nexttok,10);
		
		temp_ip[i] = num;
		nexttok = NULL;
	} 
    if ((temp_ip[0] == 0)||(temp_ip[0] > 255)||(temp_ip[1] > 255)||(temp_ip[2] > 255)||(temp_ip[3] > 255)) return;  //! IP ���ִ���255�ҵ�һ������Ϊ0 �Ƿ�
    
    ip[0] = (uint8_t)temp_ip[0];
    ip[1] = (uint8_t)temp_ip[1];
    ip[2] = (uint8_t)temp_ip[2];
    ip[3] = (uint8_t)temp_ip[3];

}	
 



/**
*@brief	 	��32��ַת��Ϊʮ���Ƹ�ʽ
*@param		addr:  Ҫת����ַ
*@return	����ʮ���Ƹ�ʽ��ַ
*/
char* inet_ntoa(unsigned long addr)
{
	static char addr_str[32];
	memset(addr_str, 0, 32);
	sprintf(addr_str, "%d.%d.%d.%d", (int)(addr>>24 & 0xFF), (int)(addr>>16 & 0xFF), (int)(addr>>8 & 0xFF), (int)(addr & 0xFF));
	return addr_str;
}





/**
*@brief	 	��16��ַת��Ϊʮ���Ƹ�ʽ
*@param		addr:  Ҫת����ַ
*@return	����ʮ���Ƹ�ʽ��ַ
*/
char* inet_ntoa_pad(unsigned long addr)
{
	static char addr_str[16];
	memset(addr_str, 0, 16);
	printf(addr_str, "%03d.%03d.%03d.%03d", (int)(addr>>24 & 0xFF), (int)(addr>>16 & 0xFF), (int)(addr>>8 & 0xFF), (int)(addr & 0xFF));
	return addr_str;
}
 



/**
*@brief	 	��֤IP��ַ
*@param		ip addr
*@return	�ɹ� - 1, ʧ�� - 0
*/
char verify_ip_address(char* src, uint8_t* ip)
{
	int i;
	int tnum;
	char tsrc[50];
	char* tok = tsrc;
	
	strcpy(tsrc, src);
	
	for (i = 0; i < 4; i++) {
		tok = strtok(tok, ".");
		if (!tok) return 0;
		if (tok[0] == '0' && tok[1] == 'x') {
			if (!validatoi(tok+2, 0x10, &tnum)) return 0;
		} else if (!validatoi(tok, 10, &tnum)) return 0;

		ip[i] = tnum;
		
		if (tnum < 0 || tnum > 255) return 0;
		tok = NULL;
	}
	return 1;	
}




/**
*@brief		��һ�� ����ģʽ��unsigned short������ת�������ģʽ��TCP/IP �����ֽڸ�ʽ������.
*@param		Ҫת��������
*@return 	���ģʽ������
*/ 
uint16_t htons(uint16_t hostshort)	/**< A 16-bit number in host byte order.  */	
{
#if (SYSTEM_ENDIAN == _ENDIAN_LITTLE_)
	return swaps(hostshort);
#else
	return hostshort;
#endif		
}




/**
*@brief		��һ�� ����ģʽ��unsigned long������ת�������ģʽ��TCP/IP �����ֽڸ�ʽ������.
*@param		Ҫת��������
*@return 	���ģʽ������
*/ 
uint32_t htonl(unsigned long hostlong)		/**< hostshort  - A 32-bit number in host byte order.  */
{
#if (SYSTEM_ENDIAN == _ENDIAN_LITTLE_)
	return swapl(hostlong);
#else
	return hostlong;
#endif	
}





/**
*@brief		��һ�����ģʽ��TCP/IP �����ֽڸ�ʽ������ת��������ģʽ��unsigned short������
*@param		Ҫת��������
*@return 	unsigned shortģʽ������
*/ 
uint32_t ntohs(unsigned short netshort)	/**< netshort - network odering 16bit value */	
{
#if (SYSTEM_ENDIAN == _ENDIAN_LITTLE_)	
	return htons(netshort);
#else
	return netshort;
#endif		
}





/**
*@brief		��һ�����ģʽ��TCP/IP �����ֽڸ�ʽ������ת��������ģʽ��unsigned long������
*@param		Ҫת��������
*@return 	unsigned longģʽ������
*/ 
uint32_t ntohl(unsigned long netlong)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return htonl(netlong);
#else
	return netlong;
#endif		
}




/**
*@brief		�����ַ���У��ֵ
*@param		Ҫת��������
*@return 	У��ֵ
*/ 
unsigned short checksum(unsigned char * src, 	/**< pointer to stream  */
	unsigned int len		/**< size of stream */
	)
{
	u_int sum, tsum, i, j;
	u_long lsum;

	j = len >> 1;

	lsum = 0;

	for (i = 0; i < j; i++) {
		tsum = src[i * 2];
		tsum = tsum << 8;
		tsum += src[i * 2 + 1];
		lsum += tsum;
	}

	if (len % 2) {
		tsum = src[i * 2];
		lsum += (tsum << 8);
	}


	sum = lsum;
	sum = ~(sum + (lsum >> 16));
	return (u_short) sum;	
}



/*******************************************************************************
* ??? : convertHexToStr
*******************************************************************************/
void convertHexToStr(uint8_t dest[], uint8_t src[], uint8_t len)
{
    uint8_t i = 0;
    uint8_t temp_h;
    uint8_t temp_l;
	
    for (i = 0; i < len; i++) {
        temp_h = src[i]>>4;
        temp_l = src[i]&0X0F;
       
		if (temp_h > 9) {
            dest[2*i] = (src[i]>>4) + 55;
			
		} else {
            dest[2*i] = (src[i]>>4) + 48;
		}
		
        if (temp_l > 9) {
            dest[2*i+1] = (src[i]&0X0F) + 55;
			
		} else {
            dest[2*i+1] = (src[i]&0X0F) + 48;
		}
    }
	
	dest[2*len] = '\0';
}


