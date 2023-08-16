#include "flash.h"

uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�

/**
  * @brief  Unlock the FLASH control register access
  * @retval LL Status
  */
LL_StatusTypeDef LL_Flash_Unlock(void)
{
	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
  {
    /* Authorize the FLASH Registers access */
    WRITE_REG(FLASH->KEYR, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR, FLASH_KEY2);

    /* Verify Flash is unlocked */
    if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
    {
      return LL_ERROR;
    }
  }
	else
		return LL_ERROR;
	return LL_OK;
}

/**
  * @brief  Locks the FLASH control register access
  * @retval LL Status
  */
LL_StatusTypeDef LL_FLASH_Lock(void)
{
  /* Set the LOCK Bit to lock the FLASH Registers access */
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);
  
  return LL_OK;
}

/**
  * @brief  Get the FLASH Status
  * @retval LL Status
  */
LL_StatusTypeDef LL_FLASH_GetStatus(void)
{	
	uint32_t res;		
	res=FLASH->SR;
	if(res&(1<<0))return LL_BUSY;		    	//æ
	else if(res&(1<<2))return LL_ERROR;		//��̴���
	else if(res&(1<<4))return LL_PROTECT;	//д��������
	return LL_OK;													//�������
}

LL_StatusTypeDef LL_FLASH_Wait(void)
{
	LL_StatusTypeDef res;
	uint16_t time = 50000;
	res = LL_FLASH_GetStatus();
	while(res == LL_BUSY)
	{
		res = LL_FLASH_GetStatus();
		if(time==0)
		{
			res = LL_TIMEOUT;
			break;
		}
		time--;
	}
	 return res;
}

LL_StatusTypeDef LL_FLASH_ErasePage(uint32_t PageAddress)
{
	LL_StatusTypeDef res;
	res = LL_FLASH_Wait();
	
	if(res == LL_OK)
	{
		SET_BIT(FLASH->CR, FLASH_CR_PER);			//ҳ����
		WRITE_REG(FLASH->AR, PageAddress);		//����ҳ��ַ
		SET_BIT(FLASH->CR, FLASH_CR_STRT);		//��ʼ����
		res = LL_FLASH_Wait();								//�ȴ���������
		if(res != LL_BUSY)
			FLASH->CR &= ~FLASH_CR_PER;					//���PERλ
	}
	return res;
}

LL_StatusTypeDef LL_FLASH_ProgramHalfWord(uint32_t Address,uint16_t Data)
{
	LL_StatusTypeDef res;
	res=LL_FLASH_Wait();
	if(res == LL_OK)
	{
		SET_BIT(FLASH->CR, FLASH_CR_PG);			//���ʹ��
		*(__IO uint16_t*)Address = Data;			//д������
		res=LL_FLASH_Wait();									//�ȴ�����
		if(res != LL_BUSY)										//�����ɹ�
			FLASH->CR &= ~FLASH_CR_PG;					//���PGλ
	}
	return res;
}

#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	uint16_t i;
	for(i=0;i<NumToWrite;i++)
	{
		LL_FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	  WriteAddr+=2;//��ַ����2
	}
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=LL_FLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
	uint32_t secpos;	   	//������ַ
	uint16_t secoff;	   	//������ƫ�Ƶ�ַ(16λ�ּ���)
	uint16_t secremain; 	//������ʣ���ַ(16λ�ּ���)	   
 	uint16_t i;    
	uint32_t offaddr;   	//ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	LL_Flash_Unlock();										//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;				//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1)
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)	//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)				//��Ҫ����
		{
			LL_FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain*2;	//д��ַƫ��(16λ���ݵ�ַ,��Ҫ*2)	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	}
	LL_FLASH_Lock();//����
}
#endif

//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
uint16_t LL_FLASH_ReadHalfWord(uint32_t faddr)
{
	return *(__IO uint16_t*)faddr;
}

uint32_t LL_FLASH_ReadWord(uint32_t faddr)
{
	uint32_t i = 0;
	i = LL_FLASH_ReadHalfWord(faddr+2);
	i = i<<16;
	i |= LL_FLASH_ReadHalfWord(faddr);
	return i;
}

void LL_FLASH_WriteHalfWord(uint32_t WriteAddr,uint16_t data)
{
	STMFLASH_Write(WriteAddr,&data,1);
}

void LL_FLASH_WriteWord(uint32_t WriteAddr,uint32_t data)
{
	uint16_t buffer[2] = {data,data>>16};
	STMFLASH_Write(WriteAddr,buffer,2);
}

