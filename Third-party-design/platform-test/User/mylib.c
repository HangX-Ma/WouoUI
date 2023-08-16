#include "mylib.h"

#define IS_BIT_SET(REG, BIT)         (((REG) & (BIT)) != 0U)
#define IS_BIT_CLR(REG, BIT)         (((REG) & (BIT)) == 0U)

int32_t not_int32(int32_t n)
{                 
	return (~n + 1); 
}

float map_number(float num, float minInput, float maxInput, float minOutput, float maxOutput)
{
//		 = 0.0;    	// ���뷶Χ����Сֵ
//		 = 4096.0; 	// ���뷶Χ�����ֵ
//		 = 0.0;   	// �����Χ����Сֵ
//		 = 32.0;  	// �����Χ�����ֵ
    // ������ӳ��
    float mappedNum = ((num - minInput) / (maxInput - minInput)) * (maxOutput - minOutput) + minOutput;
    return mappedNum;
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	LL_USART_TransmitData8(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (LL_USART_IsActiveFlag_TXE(pUSARTx) == RESET);		//��������ʹ��TXE��־���
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(LL_USART_IsActiveFlag_TC(pUSARTx)==RESET);		//�໺����ʹ��TC��־���
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* �ȴ�������� */
	while(LL_USART_IsActiveFlag_TC(pUSARTx)==RESET);
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	LL_USART_TransmitData8(pUSARTx,temp_h);	
	while (LL_USART_IsActiveFlag_TXE(pUSARTx) == RESET);
	
	/* ���͵Ͱ�λ */
	LL_USART_TransmitData8(pUSARTx,temp_l);	
	while (LL_USART_IsActiveFlag_TXE(pUSARTx) == RESET);
}

int fputc(int c,FILE *stream)
{	   
  LL_USART_TransmitData8(USART3,c);
  while(!LL_USART_IsActiveFlag_TXE(USART3))__NOP();
  LL_USART_ClearFlag_TC(USART3); 
  return c;
}

int fgetc(FILE *stream)
{
  while(!LL_USART_IsActiveFlag_RXNE(USART3)) __NOP();
  return ((char)LL_USART_ReceiveData8(USART3));
}


//�����λ
void SoftReset(void)
{
	__set_FAULTMASK(1); // �ر������ж�
	NVIC_SystemReset(); // ��λ
}

//����ջ����ַ
//addr:ջ����ַ
//__asm void MSR_MSP(uint32_t addr)
//{
////	MSR MSP, r0 			//set Main Stack value
////	BX r14
//}

