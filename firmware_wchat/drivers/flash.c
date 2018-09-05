
#include "board.h"
#include "flash.h"
#include "data_struct.h"



/**
  * @brief  get the cpu unique ID
  * @note   
  * @retval None
  */
bool getCpuId(uint32_t *mac_0, uint32_t *mac_1, uint32_t *mac_2)
{
	*mac_0 = *(uint32_t*)(0X1FFF7A10);
	*mac_1 = *(uint32_t*)(0X1FFF7A14);
	*mac_2 = *(uint32_t*)(0X1FFF7A18);
	
	if ((*mac_0 != 0) || (*mac_1 != 0) || (*mac_2 != 0)) {
		return true;
		
	} else {
		return false;
	}
	
}


void getMqttID(uint8_t id[])
{
    static uint32_t ID[3] = {0};
    uint32_t mac_0, mac_1, mac_2;
	mac_0 = *(uint32_t*)(0X1FFF7A10);
	mac_1 = *(uint32_t*)(0X1FFF7A14);
	mac_2 = *(uint32_t*)(0X1FFF7A18);
	memcpy(&ID[0], &mac_0, sizeof(mac_0));
    memcpy(&ID[1], &mac_1, sizeof(mac_1));
    memcpy(&ID[2], &mac_2, sizeof(mac_2));
    memcpy(id, ID, sizeof(ID));
}


/**
  * @brief  get the dev mac addr 
  * @note   
  * @retval None
  */
bool getDevMac(uint8_t *mac)
{
    
	uint32_t mac_0;
	uint32_t mac_1;
	uint32_t mac_2;
    uint32_t mac_t;
    uint8_t str[24] = {0};

	if (getCpuId(&mac_0, &mac_1, &mac_2)) {        
        memcpy(&str[0],  &mac_0, sizeof(mac_0));
        memcpy(&str[4],  &mac_1, sizeof(mac_1));
        memcpy(&str[8],  &mac_2, sizeof(mac_2));
        memcpy(&str[12], &mac_0, sizeof(mac_0));
        memcpy(&str[16], &mac_1, sizeof(mac_1));
        memcpy(&str[20], &mac_2, sizeof(mac_2));
        mac_t = crc32(str, sizeof(str));
        
		*mac	    = 0X4C; 
		*(mac+1)	= 0X00; 
		*(mac+2)	= (uint8_t)(mac_t >> 24);
		*(mac+3)	= (uint8_t)(mac_t >> 16);
		*(mac+4)	= (uint8_t)(mac_t >>  8);
		*(mac+5)	= (uint8_t)(mac_t);
		return true;

	} else {
        
		return false;
	}		
}





/**
*@brief		STM32系统软复位函数
*@param		无
*@return	无
*/
void systemReboot(void)
{
	pFunction Jump_To_Application;
	uint32_t JumpAddress;

	printf("system reboot! \n");    
//    __disable_irq();
//	JumpAddress = *(vu32*) (0x08000004);
//	Jump_To_Application = (pFunction) JumpAddress;
//    __set_MSP(*(__IO uint32_t*) 0x08000000);
//	Jump_To_Application();
    NVIC_SystemReset();
}




void getFlashSize(void)
{	
    uint16_t STM32_FLASH_SIZE = *(uint16_t*)(0x1FFF7A22); 
    
    printf("The chip flash size is: %d Kb \n", STM32_FLASH_SIZE);
    printf("The code burned date: "__DATE__"  @  "__TIME__"\n"); 	
    printf("The System Core Clock: %dHz.\n", SystemCoreClock);
  
}




/******************************************************
*	函 数 名: flashReadHalfWord
*	功能说明: 读取指定地址的半字
*	形    参: addr  指定地址
*	返 回 值: 半字数据
*******************************************************/
uint16_t flashReadHalfWord(uint32_t addr)  
{  
    return *(uint16_t*)addr;   
}




/******************************************************
*	函 数 名: flashReadWord
*	功能说明: 读取指定地址的字
*	形    参: addr  指定地址
*	返 回 值: 字数据
*******************************************************/
uint32_t flashReadWord(uint32_t addr)  
{  
    return *(uint32_t*)addr;   
}




/******************************************************
*	函 数 名: flashReadWords
*	功能说明: 从指定地址读出数据
*	形    参: readAddr  指定要读取的的flash区域的地址
*   形    参：pBuf      存储数据的首地址
*   形    参：num       要读取的数据个数
*******************************************************/ 
void flashReadWords(uint32_t readAddr, uint32_t *pBuf, uint32_t num)           
{    
    for (uint32_t i = 0; i < num; i++) {  
        pBuf[i] = flashReadWord(readAddr); 
        readAddr += 4;     
    }  
} 





/******************************************************
*	函 数 名: flashGetSector
*	功能说明: 根据地址计算扇区首地址
*	形    参: Address  指定地址
*	返 回 值: 扇区首地址
*******************************************************/
uint32_t flashGetSector(uint32_t addr)
{
	uint32_t sector = 0;

	if ((addr < ADDR_FLASH_SECTOR_1) && (addr >= ADDR_FLASH_SECTOR_0)) {
		sector = FLASH_Sector_0; 
        
	} else if ((addr < ADDR_FLASH_SECTOR_2) && (addr >= ADDR_FLASH_SECTOR_1)) {
		sector = FLASH_Sector_1;  
        
	} else if ((addr < ADDR_FLASH_SECTOR_3) && (addr >= ADDR_FLASH_SECTOR_2)){
		sector = FLASH_Sector_2;  
        
	} else if ((addr < ADDR_FLASH_SECTOR_4) && (addr >= ADDR_FLASH_SECTOR_3)) {
		sector = FLASH_Sector_3;
        
	} else if ((addr < ADDR_FLASH_SECTOR_5) && (addr >= ADDR_FLASH_SECTOR_4)) {
		sector = FLASH_Sector_4;
        
	} else if ((addr < ADDR_FLASH_SECTOR_6) && (addr >= ADDR_FLASH_SECTOR_5)) {
		sector = FLASH_Sector_5; 
        
	} else if ((addr < ADDR_FLASH_SECTOR_7) && (addr >= ADDR_FLASH_SECTOR_6)) {
		sector = FLASH_Sector_6;  
        
	} else if ((addr < ADDR_FLASH_SECTOR_8) && (addr >= ADDR_FLASH_SECTOR_7)) {
		sector = FLASH_Sector_7;  
	}
    
	return sector;
}




/******************************************************
*	函 数 名: flashEraseSector
*	功能说明: 根据地址计算扇区首地址,并擦除该扇区所有数据
*	形    参: Address  指定地址
*	返 回 值: 
*******************************************************/
bool flashEraseSector(uint32_t addr)
{
    uint32_t sector = flashGetSector(addr);
    //printf("sector: %X \n", sector);
    FLASH_Unlock();
    if (FLASH_EraseSector(sector, VoltageRange_3) != FLASH_COMPLETE) {
        FLASH_Lock();
        return false;
        
    } else {
        FLASH_Lock();
        return true;
    }
}




/***********************************************
*	函 数 名: flashWriteWords
*	功能说明: 向flash中写数据，4字节对齐
*	形    参: addr          flash起始地址
*             data          数据地址
*             num           数据长度（4字节）
*	返 回 值: 
**************************************************/
bool flashWriteWords(uint32_t addr, uint32_t *data, uint32_t num)
{
	for (uint32_t i = 0; (i < num) && (addr <= (ADDR_FLASH_END-4)); i++) {
		if (FLASH_ProgramWord(addr, *(uint32_t*)(data+i)) == FLASH_COMPLETE) {
            if (*(uint32_t*)addr != *(uint32_t*)(data+i)) {				
                return false;/* 读出的和写入的不相同 */
            }
			addr += 4;
            
		} else {			
			return false; /* 向flash中写数据时发生错误 */
		}
	}
	return true;
}




/***********************************************
*	函 数 名: flashInit
*	功能说明: flash解锁后可进行写操作
*	形    参: 无
*	返 回 值: 
***************************************************/
void flashInit(void)
{ 
	FLASH_Unlock();   
	FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR   | FLASH_FLAG_WRPERR | 
				    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR  | FLASH_FLAG_PGSERR);
}



void flashTest(void)
{
    uint32_t read;

    for (uint32_t i = 0; i < 131072; i += 4) {
        read = flashReadWord(ADDR_FLASH_TEMP+i);
        printf("%08X ", read);
    }
    
}



void pvdInit(void) 
{
    uint8_t level = FLASH_OB_GetBOR();
    
    if (level != OB_BOR_LEVEL1) {
        FLASH_OB_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR);
        FLASH_OB_BORConfig(OB_BOR_LEVEL1);
        FLASH_OB_Launch();
        FLASH_OB_Lock();
    }    
}


