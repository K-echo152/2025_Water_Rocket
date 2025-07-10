#include "stm32f10x.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"
#include "Delay.h"
#include <math.h>

#define mpuI2C_SCL_GPIO GPIOB
#define mpuI2C_SCL_PIN GPIO_Pin_11
#define mpuI2C_SDA_GPIO GPIOB
#define mpuI2C_SDA_PIN GPIO_Pin_10

void mpuI2C_W_SCL(uint8_t BitValue)	//дʱ����
{
			GPIO_WriteBit(mpuI2C_SCL_GPIO, mpuI2C_SCL_PIN,	(BitAction)BitValue);
			Delay_us(1);//�����Ƶ��ֲ
}

void mpuI2C_W_SDA(uint8_t BitValue)	//д������
{
			GPIO_WriteBit(mpuI2C_SDA_GPIO, mpuI2C_SDA_PIN,	(BitAction)BitValue);
			Delay_us(1);
}

uint8_t	mpuI2C_R_SDA(void)	//��������
{
			uint8_t BitValue;
			BitValue = GPIO_ReadInputDataBit(mpuI2C_SDA_GPIO, mpuI2C_SDA_PIN);
			Delay_us(1);
			return BitValue;
}

void mpuI2C_Init(void)	//I2C��ʼ��
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��SCL��SDA��ʼ��Ϊ��©���ģʽ
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
			GPIO_InitStructure.GPIO_Pin = mpuI2C_SCL_PIN;
			GPIO_Init(mpuI2C_SCL_GPIO, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = mpuI2C_SDA_PIN;
			GPIO_Init(mpuI2C_SDA_GPIO, &GPIO_InitStructure);
	
			GPIO_SetBits(mpuI2C_SCL_GPIO, mpuI2C_SCL_PIN);
			GPIO_SetBits(mpuI2C_SDA_GPIO, mpuI2C_SDA_PIN);
}

//��I2C��Ӧ��6��ʱ�������Ԫ��Ӧ
//SCLʵ�����Ƕ�д����ı��֣�SCLΪ��ʱ������������д��SCLΪ��ʱ�������������
//���ߵ�ƽʱ���ݱ��޸ģ��������Ե�Ϊ��ʼ����ֹ

void mpuI2C_Start(void)
{
			//��ʼ������SCL��SDA�ߵ�ƽʱ��SDA�Ӹߵ�ƽ�л����͵�ƽ,SCL�ٱ�Ϊ�͵�ƽ
			mpuI2C_W_SDA(1);//���ͷ�SDA�Ƚϱ��գ�Ϊ�����ظ���ʼ��������SCLΪ�͵�ƽ��SDA��ȷ����
											//��SDAҲΪ��ʱ�����ͷ�SCL�����ͷ�SDAΪ��ֹ���������»���
	
			mpuI2C_W_SCL(1);
			mpuI2C_W_SDA(0);
			mpuI2C_W_SCL(0);
}

void mpuI2C_Stop(void)
{
			//��ֹ������SCL��SDA�͵�ƽʱ��SCL�ӵ͵�ƽ�л����ߵ�ƽ,SDA�ٱ�Ϊ�ߵ�ƽ
			mpuI2C_W_SDA(0);//������SDA�Ƚϱ��գ���ȷ������������
			mpuI2C_W_SCL(1);
			mpuI2C_W_SDA(1);
}

void mpuI2C_SendByte(uint8_t Byte)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬��������λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ����ӻ���SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			
			/*��������־�⣬SCL����0���������Կ�ʼ������SCLΪ0*/
			
			for(uint8_t i = 0; i < 8; i ++)
			{
				mpuI2C_W_SDA(Byte & (0x80 >> i));
				mpuI2C_W_SCL(1);
				mpuI2C_W_SCL(0);	//���ص��ĸ����ϣ�����������ʱ
			}
}

uint8_t mpuI2C_ReceiveByte(void)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬�ӻ�����λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ���������SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			//							������Ҫ���գ�����ÿ�ζ�ȡǰҪ�ͷ�SDA��������Ȩ�����ӻ�
			
			uint8_t Byte = 0x00;
	
			mpuI2C_W_SDA(1);
			for(uint8_t i = 0; i < 8; i ++)
			{
				
				mpuI2C_W_SCL(1);
				if(mpuI2C_R_SDA()) Byte |= (0x80 >> i);
				mpuI2C_W_SCL(0);
			}
			
			return Byte;
}

void mpuI2C_SendAck(uint8_t AckBit)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӷ���һλ����
			//					0��ʾ�յ���1��ʾδ�յ�
			mpuI2C_W_SDA(AckBit);	
			mpuI2C_W_SCL(1);
			mpuI2C_W_SCL(0);
}

uint8_t mpuI2C_ReceiveAck(void)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӽ���һλ����
			//					0��ʾ�ӻ��յ���1��ʾδ�յ�
			//					����ǰ���ͷ�SDA������Ȩ���ӻ�
			
			uint8_t AckBit;
			mpuI2C_W_SDA(1);
			mpuI2C_W_SCL(1);
			AckBit = mpuI2C_R_SDA();
			mpuI2C_W_SCL(0);
	
			return AckBit;
}

void MPU6050_WriteByte2Reg(uint8_t RegAddress, uint8_t Byte)
{
			//ָ���ӻ���ַдָ���Ĵ�����ƴ��ʱ��start -> ��MPU6050��ַ -> ����Ӧ�� -> 
			//																		��ָ���Ĵ�����ַ -> ����Ӧ�� -> ��ָ������
			//																		-> ����Ӧ�� -> stop
			
			mpuI2C_Start();
			mpuI2C_SendByte(MPU6050_Address);
			mpuI2C_ReceiveAck();
			mpuI2C_SendByte(RegAddress);
			mpuI2C_ReceiveAck();	//ָ����ַ
	
			mpuI2C_SendByte(Byte);
			mpuI2C_ReceiveAck();
			mpuI2C_Stop();
}

uint8_t MPU6050_ReadByte2Reg(uint8_t RegAddress)
{
			//ָ���ӻ���ַ��ָ���Ĵ�����ƴ��ʱ��start -> ��MPU6050��ַ -> ����Ӧ�� -> 
			//																		��ָ���Ĵ�����ַ -> ����Ӧ�� -> restart 
			//																		-> ��ָ����дλ -> ����Ӧ�� -> ��������
			//																		-> ����Ӧ�� -> stop

			uint8_t Byte;
			
			mpuI2C_Start();
			mpuI2C_SendByte(MPU6050_Address);
			mpuI2C_ReceiveAck();
			mpuI2C_SendByte(RegAddress);
			mpuI2C_ReceiveAck();	//ָ����ַ
			
			mpuI2C_Start();
			mpuI2C_SendByte(MPU6050_Address | 0x01);	//ָ���ӻ���ַ���дλ����Ϊ��
			mpuI2C_ReceiveAck();
			Byte = mpuI2C_ReceiveByte();
			mpuI2C_SendAck(1);	//��Ϊ����ֻ��һ���ֽڣ����Բ���Ӧ������Ҫ��ȡ��������Ӧ��
			mpuI2C_Stop();
			
			return Byte;
}

//void MPU6050_WriteData2Reg(uint8_t RegAddress, uint8_t Data)
//{
//			//ָ���ӻ���ַдָ���Ĵ�������λ
//}

//uint8_t MPU6050_ReadData2Reg(uint8_t RegAddress)
//{
//			//ָ���ӻ���ַ��ָ���Ĵ�������λ
//			return 0x00;
//}

void MPU6050_Init(void)
{
			mpuI2C_Init();
			
/*����ʵ����Ŀ�������*/
	
			//��Դ����Ĵ���1
			//0				0				0				0			 	0(��ʧ��)	000/001
			//����λ	��˯��	��ѭ��	�޹�λ	�¶ȴ�		ѡʱ��
			MPU6050_WriteByte2Reg(MPU6050_PWR_MGMT_1, 0x01);	
	
			//��Դ����Ĵ���2
			//00(����Ҫ)		000000(���ᶼ��)
			//ѭ������Ƶ��	ÿ�������λ
			MPU6050_WriteByte2Reg(MPU6050_PWR_MGMT_2, 0x00);
	
			//�����ʷ�Ƶ�Ĵ���1
			//0x09��ʮ��Ƶ����Ƶϵ��=1+�üĴ���ֵ��
			//��С�����������������ԽСԽ��
			MPU6050_WriteByte2Reg(MPU6050_SMPLRT_DIV, 0x09);
	
			//���üĴ���
			//00		 000(����Ҫ)	110(��ƽ��)
			//�޹�λ	�ⲿͬ��		���ֵ�ͨ�˲�
			MPU6050_WriteByte2Reg(MPU6050_CONFIG, 0x06);
			
			//���������üĴ���
			//000(����)	11(���)		000
			//�Բ�ʹ��	������ѡ��	�޹�λ		(����ѡ��250��500��1000��2000 ��/s)
			MPU6050_WriteByte2Reg(MPU6050_GYRO_CONFIG, 0x18);
			
			//���ٶȼ����üĴ���
			//000(����)	11(���)		00(����)
			//�Բ�ʹ��	������ѡ��	��ͨ�˲�	(����ѡ��2��4��8��16 g)
			MPU6050_WriteByte2Reg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_ReadID(void)
{
			return MPU6050_ReadByte2Reg(MPU6050_WHO_AM_I);
}


void MPU6050_GetData(MPU6050_Data* Data)	//��ȡ6�����ݣ��ֱ��ʾXYZ���ٶ���������ֵ
{
			//��ȡX����ٶȼĴ����߰�λ��Ͱ�λ
			Data->AX = ((int16_t)MPU6050_ReadByte2Reg(MPU6050_ACCEL_XOUT_H) << 8) + MPU6050_ReadByte2Reg(MPU6050_ACCEL_XOUT_L);
	
			//��ȡY��
			Data->AY = ((int16_t)MPU6050_ReadByte2Reg(MPU6050_ACCEL_YOUT_H) << 8) + MPU6050_ReadByte2Reg(MPU6050_ACCEL_YOUT_L);
	
			//��ȡZ��
			Data->AZ = ((int16_t)MPU6050_ReadByte2Reg(MPU6050_ACCEL_ZOUT_H) << 8) + MPU6050_ReadByte2Reg(MPU6050_ACCEL_ZOUT_L);
	
			//��ȡX�������ǼĴ����߰�λ��Ͱ�λ
			Data->GX = ((int16_t)MPU6050_ReadByte2Reg(MPU6050_GYRO_XOUT_H) << 8) + MPU6050_ReadByte2Reg(MPU6050_GYRO_XOUT_L);
	
			//��ȡY��
			Data->GY = ((int16_t)MPU6050_ReadByte2Reg(MPU6050_GYRO_YOUT_H) << 8) + MPU6050_ReadByte2Reg(MPU6050_GYRO_YOUT_L);
	
			//��ȡZ��
			Data->GZ = ((int16_t)MPU6050_ReadByte2Reg(MPU6050_GYRO_ZOUT_H) << 8) + MPU6050_ReadByte2Reg(MPU6050_GYRO_ZOUT_L);
}

float MPU6050_CaculateAccel(MPU6050_Data *Data)	//
{
			//MPU6050��ȡ�������ݻ��㣺
	
			//��ȡ��������  _	 		 X
			// ----------   _  ---------
			//   32768					 ������
			
			//��ʼ��ʱ������ľ�Ϊ�������
	
			return sqrt((Data->AX)*(Data->AX)+(Data->AY)*(Data->AY)+(Data->AZ)*(Data->AZ)) * 0.00048828125;  //������Ľ����ʾ���ٶ��ж���g
}

