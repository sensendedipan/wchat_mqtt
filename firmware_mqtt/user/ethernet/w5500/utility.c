/**
******************************************************************************
* @file    			utility.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-02-14
* @brief 				公共实用函数 延时函数 数据格式转换 
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



static uint8_t  fac_us = 0;						/*us延时倍乘数*/
static uint16_t fac_ms = 0;						/*ms延时倍乘数*/




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
		
	tStart = DWT_CYCCNT;                                     /* 刚进入时的计数器值 */
	tCnt = 0;
	tDelayCnt = time_us * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */	
	}
}
#endif


/***************************************
*	函 数 名: bsp_DelayMS
*	功能说明: 为了让底层驱动在带RTOS和裸机情况下有更好的兼容性
*             专门制作一个阻塞式的延迟函数，在底层驱动中ms毫秒延迟主要用于初始化，并不会影响实时性。
*	形    参: n 延迟长度，单位1 ms
*	返 回 值: 无
****************************************/
#if 1
void delay_ms(uint32_t time_ms)
{
	delay_us(1000*time_ms);
}
#endif


/**
*@brief		初始化延迟函数
*@param		SYSCLK:系统时钟
*@return	无
*/
void systick_init(uint8_t sysclk)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = sysclk/8;
	fac_ms = (uint16_t)fac_us*999;       
}								    




/**
*@brief	    毫秒延时函数
*@param		time_ms:要延时毫秒时间数
*@return	无
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
*@brief	    微秒延时函数
*@param		time_ms:要延时微秒时间数
*@return	无
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
*@brief	 	字符转转化为8位整型函数
*@param		str:要转化字符串， base:
*@return	num:返回转化后的整型数
*/
uint16_t atoi16(char* str, uint16_t base)
{
    unsigned int num = 0;
    while (*str != 0)
        num = num * base + c2d(*str++);
    return num;
}




/**
*@brief	 	字符转转化为32位整型函数
*@param		str:要转化字符串， base:
*@return	num:返回转化后的整型数
*/
uint32_t atoi32(char* str, uint16_t base)
{
    uint32_t num = 0;
    while (*str !=0)
        num = num * base + c2d(*str++);
    return num;
}




/**
*@brief	 	整型数转化为字符串函数
*@param		n:要转化整数， str[5]:存放转化后的字符串  len：整型数长度
*@return	无
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
*@brief	 	把字符串转化为十进制或十六进制数函数
*@param		str:要转化字符串， len：整型数长度
*@return	成功 - 1, 失败 - 0
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
*@brief	 	用新的字符去替换字符串中特殊的字符
*@param		str:替换后字符串，oldchar:特殊的字符，newchar：新的字符	
*@return	无
*/
void replacetochar(char* str, char oldchar, char newchar)
{
    int x;
    for (x = 0; str[x]; x++) 
        if (str[x] == oldchar) str[x] = newchar;	
}




/**
*@brief	 	把十进制数转化为字符型
*@param		c:要转化十进制数据
*@return	返回一个字符型数据
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
*@brief	 	16位字符高8位低8位转换
*@param		i:要转化的数据
*@return	转换后的数据
*/
uint16_t swaps(uint16_t i)
{
    uint16_t ret = 0;
    ret = (i & 0xFF) << 8;
    ret |= ((i >> 8)& 0xFF);
    return ret;	
}




/**
*@brief	 	32位字符高低位变换
*@param		i:要转化的数据
*@return	转换后的数据
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
*@brief	 	字符串处理
*@param		src目标字符串 s1 s2操作字符串
*@return	无
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
*@brief	 	ip网络地址转换
*@param		adr：地址 ip：ip
*@return	无
*/
void inet_addr_(unsigned char* addr, unsigned char* ip)
{
	int i;
	char taddr[16];
	char *nexttok;
	uint16_t num;
    uint16_t temp_ip[4] = {0}; //! 定义16位是为了检测出是否溢出 （255）
    unsigned char num_point = 0;
    
	strcpy(taddr, (char *)addr);
    for (uint8_t i = 0; i < 16; i++) {
        if (taddr[i] == '.') num_point++;
    }
    if (num_point != 3) return; //! '.'的个数没有三个，说明输入的ip非法
                                      
	nexttok = taddr;
	for (i = 0; i < 4 ; i++) {
		nexttok = strtok(nexttok, ".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = atoi16(nexttok+2, 0x10);
		else num = atoi16(nexttok,10);
		
		temp_ip[i] = num;
		nexttok = NULL;
	} 
    if ((temp_ip[0] == 0)||(temp_ip[0] > 255)||(temp_ip[1] > 255)||(temp_ip[2] > 255)||(temp_ip[3] > 255)) return;  //! IP 数字大于255且第一个数字为0 非法
    
    ip[0] = (uint8_t)temp_ip[0];
    ip[1] = (uint8_t)temp_ip[1];
    ip[2] = (uint8_t)temp_ip[2];
    ip[3] = (uint8_t)temp_ip[3];

}	
 



/**
*@brief	 	将32地址转化为十进制格式
*@param		addr:  要转化地址
*@return	返回十进制格式地址
*/
char* inet_ntoa(unsigned long addr)
{
	static char addr_str[32];
	memset(addr_str, 0, 32);
	sprintf(addr_str, "%d.%d.%d.%d", (int)(addr>>24 & 0xFF), (int)(addr>>16 & 0xFF), (int)(addr>>8 & 0xFF), (int)(addr & 0xFF));
	return addr_str;
}





/**
*@brief	 	将16地址转化为十进制格式
*@param		addr:  要转化地址
*@return	返回十进制格式地址
*/
char* inet_ntoa_pad(unsigned long addr)
{
	static char addr_str[16];
	memset(addr_str, 0, 16);
	printf(addr_str, "%03d.%03d.%03d.%03d", (int)(addr>>24 & 0xFF), (int)(addr>>16 & 0xFF), (int)(addr>>8 & 0xFF), (int)(addr & 0xFF));
	return addr_str;
}
 



/**
*@brief	 	验证IP地址
*@param		ip addr
*@return	成功 - 1, 失败 - 0
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
*@brief		将一个 主机模式的unsigned short型数据转换到大端模式的TCP/IP 网络字节格式的数据.
*@param		要转换的数据
*@return 	大端模式的数据
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
*@brief		将一个 主机模式的unsigned long型数据转换到大端模式的TCP/IP 网络字节格式的数据.
*@param		要转换的数据
*@return 	大端模式的数据
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
*@brief		将一个大端模式的TCP/IP 网络字节格式的数据转换到主机模式的unsigned short型数据
*@param		要转换的数据
*@return 	unsigned short模式的数据
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
*@brief		将一个大端模式的TCP/IP 网络字节格式的数据转换到主机模式的unsigned long型数据
*@param		要转换的数据
*@return 	unsigned long模式的数据
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
*@brief		计算字符串校验值
*@param		要转换的数据
*@return 	校验值
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


