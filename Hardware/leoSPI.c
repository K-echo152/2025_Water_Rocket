#include "stm32f10x.h"                  // Device header

/*�˴�Ϊ���SPI����Ҫʱ���ߡ�Ƭѡ���������ӻ��롢������ӻ���*/
/*Ĭ��ʹ��ģʽ0*/

//���SPI�ٶȷǳ��죬������I2Cһ������ʱ
void leoSPI_W_SS(uint8_t BitValue)	//дSS���������ӻ�ѡ�����
{
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void leoSPI_W_SCK(uint8_t BitValue)	//дSCK������������ʱ�ӣ����
{
			GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

void leoSPI_W_MOSI(uint8_t BitValue)	//дMOSI��������������ݣ����
{
			GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)BitValue);
}

uint8_t leoSPI_R_MISO(void)	//��MISO���������������ݣ�����
{
			return GPIO_ReadInputDataBit(GPIOA,	GPIO_Pin_4);
}

void leoSPI_Init(void)	//SPI��ʼ��
{
			//GPIO��ʼ��ͨ������
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
			GPIO_InitTypeDef GPIO_InitStructure;
	
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//���ʹ���������ģʽ
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_4;	//ʱ�ӡ���������ƬѡΪ���
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
	
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//����ʹ����������ģʽ
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//������Ϊ����
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
	
			//��ʼ���������Ĭ������
			leoSPI_W_SS(1);	//�øߣ�Ĭ�ϲ�ѡ�дӻ�
			leoSPI_W_SCK(0);	//�ƻ�ʹ��ģʽ0��Ĭ�ϵ͵�ƽ
}

//ʵ��SPI����Ĭ��ʱ��Ԫ
void leoSPI_Start(void)	//��ʼ������SS�Ӹߵ�ƽ�õ͵�ƽ
{
			leoSPI_W_SS(0);
}

void leoSPI_Stop(void)	//��ֹ������SS�ӵ͵�ƽ�øߵ�ƽ
{
			leoSPI_W_SS(1);
}

uint8_t leoSPI_SwapByte(uint8_t ByteSend)	//ģʽ0�����ֽڣ�������ģʽҪ��Ӧ����
{
			for (uint8_t i = 0; i < 8; i ++)
			{
				//2.�Ƴ�����
				leoSPI_W_MOSI(ByteSend & 0x80);	//�Ӹߵ��Ͱ�λ�Ƴ�
				ByteSend <<= 1;
				
				//3.SCK������
				leoSPI_W_SCK(1);
		
				//4.��������
				if (leoSPI_R_MISO() == 1)
				ByteSend |= 0x01;	//�Ӹߵ��Ͱ�λ����
		
				//1.SS��SCK�½���
				leoSPI_W_SCK(0);
			}
			
			return ByteSend;
}
