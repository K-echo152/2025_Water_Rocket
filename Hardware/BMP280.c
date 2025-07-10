#include "stm32f10x.h"                  // Device header
#include "BMP280.h"
#include "BMP280_Reg.h"
#include "Delay.h"
#include <math.h>

//�����ڳ�ʼ�������м�¼оƬ�ڽ�������
BMP280 bmp;
BMP280* bmp280 = &bmp;	//�ȶ�����ָ��


//////////////////////////////////////////////////////////////////////////////////////////IIC��������


#define bmpI2C_SCL_GPIO GPIOB
#define bmpI2C_SCL_PIN GPIO_Pin_13
#define bmpI2C_SDA_GPIO GPIOB
#define bmpI2C_SDA_PIN GPIO_Pin_12


//�Զ�д���к�����װ
void bmpI2C_W_SCL(uint8_t BitValue)	//дʱ����
{
			GPIO_WriteBit(bmpI2C_SCL_GPIO, bmpI2C_SCL_PIN,	(BitAction)BitValue);
			Delay_us(1);//�����Ƶ��ֲ
}

void bmpI2C_W_SDA(uint8_t BitValue)	//д������
{
			GPIO_WriteBit(bmpI2C_SDA_GPIO, bmpI2C_SDA_PIN,	(BitAction)BitValue);
			Delay_us(1);
}

uint8_t	bmpI2C_R_SDA(void)	//��������
{
			uint8_t BitValue;
			BitValue = GPIO_ReadInputDataBit(bmpI2C_SDA_GPIO, bmpI2C_SDA_PIN);
			Delay_us(1);
			return BitValue;
}

void bmpI2C_Init(void)	//I2C��ʼ��
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��SCL��SDA��ʼ��Ϊ��©���ģʽ
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
			GPIO_InitStructure.GPIO_Pin = bmpI2C_SCL_PIN;
			GPIO_Init(bmpI2C_SCL_GPIO, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = bmpI2C_SDA_PIN;
			GPIO_Init(bmpI2C_SDA_GPIO, &GPIO_InitStructure);
	
			GPIO_SetBits(bmpI2C_SCL_GPIO, bmpI2C_SCL_PIN);
			GPIO_SetBits(bmpI2C_SDA_GPIO, bmpI2C_SDA_PIN);
}


//��I2C��Ӧ��6��ʱ�������Ԫ��Ӧ
//SCLʵ�����Ƕ�д����ı��֣�SCLΪ��ʱ������������д��SCLΪ��ʱ�������������
//���ߵ�ƽʱ���ݱ��޸ģ��������Ե�Ϊ��ʼ����ֹ

void bmpI2C_Start(void)
{
			//��ʼ������SCL��SDA�ߵ�ƽʱ��SDA�Ӹߵ�ƽ�л����͵�ƽ,SCL�ٱ�Ϊ�͵�ƽ
			bmpI2C_W_SDA(1);//���ͷ�SDA�Ƚϱ��գ�Ϊ�����ظ���ʼ��������SCLΪ�͵�ƽ��SDA��ȷ����
											//��SDAҲΪ��ʱ�����ͷ�SCL�����ͷ�SDAΪ��ֹ���������»���
	
			bmpI2C_W_SCL(1);
			bmpI2C_W_SDA(0);
			bmpI2C_W_SCL(0);
}

void bmpI2C_Stop(void)
{
			//��ֹ������SCL��SDA�͵�ƽʱ��SCL�ӵ͵�ƽ�л����ߵ�ƽ,SDA�ٱ�Ϊ�ߵ�ƽ
			bmpI2C_W_SDA(0);//������SDA�Ƚϱ��գ���ȷ������������
			bmpI2C_W_SCL(1);
			bmpI2C_W_SDA(1);
}

void bmpI2C_SendByte(uint8_t Byte)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬��������λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ����ӻ���SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			
			/*��������־�⣬SCL����0���������Կ�ʼ������SCLΪ0*/
			
			for(uint8_t i = 0; i < 8; i ++)
			{
				bmpI2C_W_SDA(Byte & (0x80 >> i));
				bmpI2C_W_SCL(1);
				bmpI2C_W_SCL(0);	//���ص��ĸ����ϣ�����������ʱ
			}
}

uint8_t bmpI2C_ReceiveByte(void)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬�ӻ�����λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ���������SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			//							������Ҫ���գ�����ÿ�ζ�ȡǰҪ�ͷ�SDA��������Ȩ�����ӻ�
			
			uint8_t Byte = 0x00;
	
			bmpI2C_W_SDA(1);
			for(uint8_t i = 0; i < 8; i ++)
			{
				
				bmpI2C_W_SCL(1);
				if(bmpI2C_R_SDA()) Byte |= (0x80 >> i);
				bmpI2C_W_SCL(0);
			}
			
			return Byte;
}

void bmpI2C_SendAck(uint8_t AckBit)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӷ���һλ����
			//					0��ʾ�յ���1��ʾδ�յ�
			bmpI2C_W_SDA(AckBit);	
			bmpI2C_W_SCL(1);
			bmpI2C_W_SCL(0);
}

uint8_t bmpI2C_ReceiveAck(void)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӽ���һλ����
			//					0��ʾ�ӻ��յ���1��ʾδ�յ�
			//					����ǰ���ͷ�SDA������Ȩ���ӻ�
			
			uint8_t AckBit;
			bmpI2C_W_SDA(1);
			bmpI2C_W_SCL(1);
			AckBit = bmpI2C_R_SDA();
			bmpI2C_W_SCL(0);
	
			return AckBit;
}

void BMP280_WriteByte2Reg(uint8_t RegAddress, uint8_t Byte)
{
			//ָ���ӻ���ַдָ���Ĵ�����ƴ��ʱ��start -> ��BMP280��ַ -> ����Ӧ�� -> 
			//																		��ָ���Ĵ�����ַ -> ����Ӧ�� -> ��ָ������
			//																		-> ����Ӧ�� -> stop
			
			bmpI2C_Start();
			bmpI2C_SendByte(BMP280_ADDRESS);
			bmpI2C_ReceiveAck();
			bmpI2C_SendByte(RegAddress);
			bmpI2C_ReceiveAck();	//ָ����ַ
	
			bmpI2C_SendByte(Byte);
			bmpI2C_ReceiveAck();
			bmpI2C_Stop();
}

uint8_t BMP280_ReadByte2Reg(uint8_t RegAddress)
{
			//ָ���ӻ���ַ��ָ���Ĵ�����ƴ��ʱ��start -> ��BMP280��ַ -> ����Ӧ�� -> 
			//																		��ָ���Ĵ�����ַ -> ����Ӧ�� -> restart 
			//																		-> ��ָ����дλ -> ����Ӧ�� -> ��������
			//																		-> ����Ӧ�� -> stop

			uint8_t Byte;
			
			bmpI2C_Start();
			bmpI2C_SendByte(BMP280_ADDRESS);
			bmpI2C_ReceiveAck();
			bmpI2C_SendByte(RegAddress);
			bmpI2C_ReceiveAck();	//ָ����ַ
			
			bmpI2C_Start();
			bmpI2C_SendByte(BMP280_ADDRESS | 0x01);	//ָ���ӻ���ַ���дλ����Ϊ��
			bmpI2C_ReceiveAck();
			Byte = bmpI2C_ReceiveByte();
			bmpI2C_SendAck(1);	//��Ϊ����ֻ��һ���ֽڣ����Բ���Ӧ������Ҫ��ȡ��������Ӧ��
			bmpI2C_Stop();
			
			return Byte;
}


//////////////////////////////////////////////////////////////////////////////////////////bmp280��������


void BMP280_Init(void)	//BMP280��ʼ��
{
			bmpI2C_Init();
	
			//���¶ȴ���������ֵ
			bmp280->T1 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_T1_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_T1_LSB_REG);			//��λ�ӵ�λ
			bmp280->T2 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_T2_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_T2_LSB_REG);
			bmp280->T3 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_T3_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_T3_LSB_REG);
																																					
			//����ѹ����������ֵ                                                
			bmp280->P1 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P1_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P1_LSB_REG);
			bmp280->P2 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P2_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P2_LSB_REG);
			bmp280->P3 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P3_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P3_LSB_REG);
			bmp280->P4 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P4_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P4_LSB_REG);
			bmp280->P5 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P5_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P5_LSB_REG);
			bmp280->P6 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P6_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P6_LSB_REG);
			bmp280->P7 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P7_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P7_LSB_REG);
			bmp280->P8 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P8_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P8_LSB_REG);
			bmp280->P9 = (((uint16_t)BMP280_ReadByte2Reg(BMP280_DIG_P9_MSB_REG))<<8) | BMP280_ReadByte2Reg(BMP280_DIG_P9_LSB_REG);
			
			//��λ
			BMP280_WriteByte2Reg(BMP280_RESET_REG, BMP280_RESET_VALUE);
	
			//���ò������ƼĴ���
			//	001	(x1)	 		011(x4)			11(normal)
			//�¶ȹ�����  	ѹ��������  	����ģʽ
			BMP280_WriteByte2Reg(BMP280_CTRLMEAS_REG, 0x2F);
			
			
			//����config�Ĵ���
			//000(0.5ms)			100(x16)					0				0(������)
			//����ʱ��SB	 	�˲�����Ƶ����		�޹�λ		SPI����
			BMP280_WriteByte2Reg(BMP280_CONFIG_REG, 0x10);
}

void BMP280_WaitRead(void)	//�ȴ�ת������Զ�ȡ����
{
			while (BMP280_ReadByte2Reg(BMP280_STATUS_REG) == BMP280_MEASURING 
					|| BMP280_ReadByte2Reg(BMP280_STATUS_REG) == BMP280_IM_UPDATE)	//������ڲ�������£���ȵ����Զ�ȡ
			{
				
			}
}

uint8_t BMP280_ReadID(void)
{
	return BMP280_ReadByte2Reg(BMP280_CHIPID_REG);
}

void BMP280_GetData(double *Temp, double *Pres)	//��ȡ�Ĵ����ڵ�ԭʼ���ݲ�����ת��Ϊ׼ȷֵ
{
			int32_t adc_T, adc_P, BMP280_t_fine;	//�Ĵ����ڶ�����ԭʼֵ���¶�����ѹ�Ĵ��ݱ���
			double var1, var2, p;
	
			BMP280_WaitRead();
			adc_T = (uint32_t) (BMP280_ReadByte2Reg(BMP280_TEMPERATURE_MSB_REG) << 12) 
								| (uint32_t) (BMP280_ReadByte2Reg(BMP280_TEMPERATURE_LSB_REG) << 4)
								| (uint32_t) (BMP280_ReadByte2Reg(BMP280_TEMPERATURE_XLSB_REG ) >> 4);
			
			var1 = (((double)adc_T) / 16384.0 - ((double)bmp280->T1) / 1024.0) * ((double)bmp280->T2);
			var2 = ((((double)adc_T) / 131072.0 - ((double)bmp280->T1) /8192.0) * (((double)adc_T)
                / 131072.0 - ((double)bmp280->T1) / 8192.0)) * ((double)bmp280->T3);
			
			BMP280_t_fine = (int32_t)(var1 + var2);
			*Temp = (var1 + var2) / 5120.0;	//�õ��¶�
	
			BMP280_WaitRead();
			adc_P = (int32_t) (BMP280_ReadByte2Reg(BMP280_PRESSURE_MSB_REG) << 12) 
								+ (int32_t) (BMP280_ReadByte2Reg(BMP280_PRESSURE_LSB_REG) << 4)
								+ (int32_t) (BMP280_ReadByte2Reg(BMP280_PRESSURE_XLSB_REG ) >> 4);

			var1 = ((double)BMP280_t_fine / 2.0) - 64000.0;
			var2 = var1 * var1 * ((double)bmp280->P6) / 32768.0;
			var2 = var2 + var1 * ((double)bmp280->P5) * 2.0;
			var2 = (var2 / 4.0) + (((double)bmp280->P4) * 65536.0);
			var1 = (((double)bmp280->P3) * var1 * var1 / 524288.0 + ((double)bmp280->P2) * var1) / 524288.0;
			var1 = (1.0 + var1 /32768.0) * ((double)bmp280->P1);
			if (var1 == 0.0)
			{
				
			}
			p = 1048576.0 - (double)adc_P;
			p = (p - (var2 / 4096.0)) * 6250.0 /	var1;
			var1 = ((double)bmp280->P9) * p * p / 2147483648.0;
			var2 = p * ((double)bmp280->P8) / 32768.0;
			*Pres = p + (var1 + var2 + ((double)bmp280->P7)) / 16.0;
}

uint16_t BMP280_GetHeight(void)
{
			double Temp, Pres;
			uint16_t Height;
			BMP280_GetData(&Temp, &Pres);
			Height = 153.8* (( pow( 101325.0f/ Pres, 1.0f / 5.255f)-1)*(273.2+ Temp));
			return Height;
}
