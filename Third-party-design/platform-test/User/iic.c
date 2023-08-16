#include "iic.h"
//*****************************************************************/
//��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA(HIGH);
	IIC_SCL(HIGH);
	IIC_Delay();
	IIC_SDA(LOW);
	IIC_Delay();
	IIC_SCL(LOW);
}

/*****************************************************************/
//ֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(LOW);
	IIC_SDA(LOW);
	IIC_Delay();
	IIC_SCL(HIGH);
	IIC_SDA(HIGH);
	IIC_Delay();
}

/*****************************************************************/
//�ȴ���Ӧ�ź�
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t	IIC_Wait_Ack(void)
{
	uint8_t Time = 0;

	SDA_IN();
	IIC_SDA(HIGH);
	IIC_Delay();
	IIC_SCL(HIGH);
	IIC_Delay();
	while(READ_SDA)
	{
		Time++;
		IIC_Delay();
		if(Time>250)
		{
			SDA_OUT();
			IIC_Stop();
			return 1;
		}
	}
	SDA_OUT();
	IIC_SCL(LOW);
	return 0;
}

/***************************************************************/
//������Ӧ�ź�
void IIC_Ack(void)
{
	IIC_SCL(LOW);
	SDA_OUT();
	IIC_SDA(LOW);
	IIC_Delay();
	IIC_SCL(HIGH);
	IIC_Delay();
	IIC_SCL(LOW);
}

/**************************************************************/
//��������Ӧ�ź�
void IIC_NAck(void)
{
	IIC_SCL(LOW);
	SDA_OUT();
	IIC_SDA(HIGH);
	IIC_Delay();
	IIC_SCL(HIGH);
	IIC_Delay();
	IIC_SCL(LOW);
}

/*************************************************************/
//����һ���ֽ�
void IIC_Send_Byte(uint8_t Txd)
{
	uint8_t t;
	SDA_OUT();
	IIC_SCL(LOW);
	for(t = 0;t<8;t++)
	{
		IIC_SDA((Txd&0x80)>>7);
		Txd<<=1;
		IIC_SCL(HIGH);
		IIC_Delay();
		IIC_SCL(LOW);
		IIC_Delay();
	}
}

/**************************************************************/
//��ȡһ���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC_Read_Byte(uint8_t Ack)
{
	uint8_t i,receive = 0;
	SDA_IN();
	for(i = 0;i<8;i++)
	{
		IIC_SCL(LOW);
		IIC_Delay();
		IIC_SCL(HIGH);
		IIC_Delay();
		IIC_Delay();
		receive<<=1;
		if(READ_SDA)
		{
			receive++;
		}
		IIC_Delay();
	}
	SDA_OUT();
	if(!Ack)
		IIC_NAck();
	else
		IIC_Ack();
	return receive;
}

/**************************************************************/
//IIC��ʱ
void IIC_Delay(void)
{
	//������ָ�� 1/72M=0.01389us
//	co_delay_10us(1);
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
	LL_mDelay(1);
}


void SDA_OUT(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = LCD_SDA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(LCD_SCL_GPIO_Port, &GPIO_InitStruct);
}

void SDA_IN(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = LCD_SDA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(LCD_SCL_GPIO_Port, &GPIO_InitStruct);
}

//��ʼ��IIC
void IIC_Init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = LCD_SCL_Pin|LCD_SDA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(LCD_SCL_GPIO_Port, &GPIO_InitStruct);
}



