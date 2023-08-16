#ifndef __FLASH_H
#define __FLASH_H
#include "main.h"

#define	USER_CONFIG_ADDR					STM32_FLASH_BASE+0xF000	//���±�־�洢��ַ

//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 	64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 	1       //ʹ��FLASHд��(0��������;1��ʹ��)


/**
  * @brief  HAL Status structures definition  
  */
typedef enum
{
  LL_OK       = 0x00U,
  LL_ERROR    = 0x01U,
  LL_BUSY     = 0x02U,
  LL_TIMEOUT  = 0x03U,
	LL_PROTECT  = 0x04U
} LL_StatusTypeDef;

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 		//STM32 FLASH����ʼ��ַ
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif


//LL_StatusTypeDef LL_FLASH_Lock(void);
//LL_StatusTypeDef LL_Flash_Unlock(void);

//static LL_StatusTypeDef LL_FLASH_ProgramHalfWord(uint32_t Address,uint16_t Data)
//void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);

//LL_StatusTypeDef LL_FLASH_ErasePage(uint32_t PageAddress);

void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);


uint16_t LL_FLASH_ReadHalfWord(uint32_t faddr);
uint32_t LL_FLASH_ReadWord(uint32_t faddr);

void LL_FLASH_WriteHalfWord(uint32_t WriteAddr,uint16_t data);
void LL_FLASH_WriteWord(uint32_t WriteAddr,uint32_t data);


#endif		/* __FLASH_H */

