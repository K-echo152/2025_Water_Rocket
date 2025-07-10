#include "stm32f10x.h"
#include "OLED_Data.h"
#include "Delay.h"
#include <math.h>


/*�˴���OLED����Ϊ���װ棬ֻΪ��ʾ���ֵȣ�ͼ���뺺�ְ���ͼ��洢
	û���õ��������飬��������Զ�ʵ�ֱ�ɫ����ͼ�ȱ���UI�����Ĺ���
	������������������мǣ���128*���أ���8*page*/


#define OLED_Address    0x78	//һ��OLED���ĵ�ַΪ0x78


//////////////////////////////////////////////////////////////////////////////////////////IIC��������


#define oledI2C_SCL_GPIO GPIOB
#define oledI2C_SCL_PIN GPIO_Pin_8
#define oledI2C_SDA_GPIO GPIOB
#define oledI2C_SDA_PIN GPIO_Pin_9


//�Զ�д���к�����װ
void oledI2C_W_SCL(uint8_t BitValue)	//дʱ����
{
			GPIO_WriteBit(oledI2C_SCL_GPIO, oledI2C_SCL_PIN,	(BitAction)BitValue);
			Delay_us(1);//�����Ƶ��ֲ
}

void oledI2C_W_SDA(uint8_t BitValue)	//д������
{
			GPIO_WriteBit(oledI2C_SDA_GPIO, oledI2C_SDA_PIN,	(BitAction)BitValue);
			Delay_us(1);
}

uint8_t	oledI2C_R_SDA(void)	//��������
{
			uint8_t BitValue;
			BitValue = GPIO_ReadInputDataBit(oledI2C_SDA_GPIO, oledI2C_SDA_PIN);
			Delay_us(1);
			return BitValue;
}

void oledI2C_Init(void)	//I2C��ʼ��
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��SCL��SDA��ʼ��Ϊ��©���ģʽ
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
			GPIO_InitStructure.GPIO_Pin = oledI2C_SCL_PIN;
			GPIO_Init(oledI2C_SCL_GPIO, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = oledI2C_SDA_PIN;
			GPIO_Init(oledI2C_SDA_GPIO, &GPIO_InitStructure);
	
			GPIO_SetBits(oledI2C_SCL_GPIO, oledI2C_SCL_PIN);
			GPIO_SetBits(oledI2C_SDA_GPIO, oledI2C_SDA_PIN);
}

//��I2C��Ӧ��6��ʱ�������Ԫ��Ӧ
//SCLʵ�����Ƕ�д����ı��֣�SCLΪ��ʱ������������д��SCLΪ��ʱ�������������
//���ߵ�ƽʱ���ݱ��޸ģ��������Ե�Ϊ��ʼ����ֹ

void oledI2C_Start(void)
{
			//��ʼ������SCL��SDA�ߵ�ƽʱ��SDA�Ӹߵ�ƽ�л����͵�ƽ,SCL�ٱ�Ϊ�͵�ƽ
			oledI2C_W_SDA(1);//���ͷ�SDA�Ƚϱ��գ�Ϊ�����ظ���ʼ��������SCLΪ�͵�ƽ��SDA��ȷ����
											//��SDAҲΪ��ʱ�����ͷ�SCL�����ͷ�SDAΪ��ֹ���������»���
	
			oledI2C_W_SCL(1);
			oledI2C_W_SDA(0);
			oledI2C_W_SCL(0);
}

void oledI2C_Stop(void)
{
			//��ֹ������SCL��SDA�͵�ƽʱ��SCL�ӵ͵�ƽ�л����ߵ�ƽ,SDA�ٱ�Ϊ�ߵ�ƽ
			oledI2C_W_SDA(0);//������SDA�Ƚϱ��գ���ȷ������������
			oledI2C_W_SCL(1);
			oledI2C_W_SDA(1);
}

void oledI2C_SendByte(uint8_t Byte)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬��������λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ����ӻ���SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			
			/*��������־�⣬SCL����0���������Կ�ʼ������SCLΪ0*/
			
			for(uint8_t i = 0; i < 8; i ++)
			{
				oledI2C_W_SDA(Byte & (0x80 >> i));
				oledI2C_W_SCL(1);
				oledI2C_W_SCL(0);	//���ص��ĸ����ϣ�����������ʱ
			}
}

uint8_t oledI2C_ReceiveByte(void)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬�ӻ�����λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ���������SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			//							������Ҫ���գ�����ÿ�ζ�ȡǰҪ�ͷ�SDA��������Ȩ�����ӻ�
			
			uint8_t Byte = 0x00;
	
			oledI2C_W_SDA(1);
			for(uint8_t i = 0; i < 8; i ++)
			{
				
				oledI2C_W_SCL(1);
				if(oledI2C_R_SDA()) Byte |= (0x80 >> i);
				oledI2C_W_SCL(0);
			}
			
			return Byte;
}

void oledI2C_SendAck(uint8_t AckBit)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӷ���һλ����
			//					0��ʾ�յ���1��ʾδ�յ�
			oledI2C_W_SDA(AckBit);	
			oledI2C_W_SCL(1);
			oledI2C_W_SCL(0);
}

uint8_t oledI2C_ReceiveAck(void)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӽ���һλ����
			//					0��ʾ�ӻ��յ���1��ʾδ�յ�
			//					����ǰ���ͷ�SDA������Ȩ���ӻ�
			
			uint8_t AckBit;
			oledI2C_W_SDA(1);
			oledI2C_W_SCL(1);
			AckBit = oledI2C_R_SDA();
			oledI2C_W_SCL(0);
	
			return AckBit;
}


//////////////////////////////////////////////////////////////////////////////////////////OLED��������


void OLED_WrtCmd(uint8_t Cmd)
{
			//д���start -> Ѱַ -> ����Ӧ�� -> ������ģʽ
			//				�� ctrl byte -> ����Ӧ�� -> data or byte -> ����Ӧ�� -> ��ֹ
	
			oledI2C_Start();
			oledI2C_SendByte(OLED_Address);	//Ѱַ
			oledI2C_ReceiveAck();
															//							 ���λΪ0 			�θ�λD/CΪ0�������Ϊ0
			oledI2C_SendByte(0x00);	//control byte����ʾ������ģʽ����ʾд�������Ϊ0x00
			oledI2C_ReceiveAck();
	
			oledI2C_SendByte(Cmd);	//data byte������Ϊ���㣬ֻдһ���ֽ�
			oledI2C_ReceiveAck();
	
			oledI2C_Stop();
}

void OLED_WrtData(uint8_t Data)
{
			//д���ݣ�start -> Ѱַ -> ����Ӧ�� -> ������ģʽ
			//				�� ctrl byte -> ����Ӧ�� -> data or byte -> ����Ӧ�� -> ��ֹ
	
			oledI2C_Start();
			oledI2C_SendByte(OLED_Address);	//Ѱַ
			oledI2C_ReceiveAck();
											
			oledI2C_SendByte(0x40);	//control byte��������ģʽд���ݣ��θ�λΪ1������Ϊ0x40
			oledI2C_ReceiveAck();
	
			oledI2C_SendByte(Data);	//data byte������Ϊ���㣬ֻдһ���ֽ�
			oledI2C_ReceiveAck();
	
			oledI2C_Stop();
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
			uint32_t Result = 1;	//���Ĭ��Ϊ1
			while (Y --)			//�۳�Y��
			{
				Result *= X;		//ÿ�ΰ�X�۳˵������
			}
			return Result;
}

void OLED_Init(void)//��ʼ��
{
			oledI2C_Init();
	
			Delay_ms(100);	//�ϵ縴λ����ʱ
	
			OLED_WrtCmd(0xAE);	//��ʾ�ر�
	
			OLED_WrtCmd(0xD5);
			OLED_WrtCmd(0x80);	//˫�ֽ�����
	
			OLED_WrtCmd(0xA8);
			OLED_WrtCmd(0x3F);
	
			OLED_WrtCmd(0xD3);
			OLED_WrtCmd(0x00);
	
			OLED_WrtCmd(0x40);
			
			OLED_WrtCmd(0xA1);
	
			OLED_WrtCmd(0xC8);
			
			OLED_WrtCmd(0xDA);
			OLED_WrtCmd(0x12);
			
			OLED_WrtCmd(0x81);
			OLED_WrtCmd(0xCF);
			
			OLED_WrtCmd(0xD9);
			OLED_WrtCmd(0xF1);
			
			OLED_WrtCmd(0xDB);
			OLED_WrtCmd(0x30);
			
			OLED_WrtCmd(0xA4);
			
			OLED_WrtCmd(0xA6);
			
			OLED_WrtCmd(0x8D);
			OLED_WrtCmd(0x14);
			
			OLED_WrtCmd(0xAF);
			
			Delay_ms(100);
}

void OLED_SetCursor(uint8_t X, uint8_t Page)	//���ù��
{
			//�������λΪ0000ʱ������λ��ʾX�����λ
			OLED_WrtCmd(0x00 | (X & 0x0F));
	
		
			//�������λΪ0001ʱ������λ��ʾX�����λ
			OLED_WrtCmd(0x10 | ((X & 0xF0) >> 4));
	
			//�������λΪ1011 0ʱ������λ��ʾҳ��ֵַ
			OLED_WrtCmd(0xB0 | Page);
}

void OLED_Clear(void)	//����
{
			for (uint8_t i = 0; i < 8; i ++)
			{
				OLED_SetCursor(0, i);
				for (uint8_t j = 0; j < 128; j ++)
				{
					OLED_WrtData(0x00);
				}
			}
}

void OLED_ShowChar(uint8_t X, uint8_t Page, char Char, uint8_t Size)	//��ʾ�ַ�
{
			if (Size == 6)
			{
				OLED_SetCursor(X, Page);
				
				for (uint8_t i = 0; i < 6; i ++)
				{
						OLED_WrtData(OLED_F6x8[Char - 0x20][i]);
				}
			}
			
			else if (Size == 8)
			{
				OLED_SetCursor(X, Page);
				
				for (uint8_t i = 0; i < 8; i ++)
				{
						OLED_WrtData(OLED_F8x16[Char - 0x20][i]);
				}
				
				OLED_SetCursor(X, Page + 1);
				
				for (uint8_t i = 8; i < 16; i ++)
				{
						OLED_WrtData(OLED_F8x16[Char - 0x20][i]);
				}
			}
}

void OLED_ShowString(uint8_t X, uint8_t Page, char* String, uint8_t Size)	//��ʾ�ַ���
{
				for (uint8_t i = 0; String[i] != '\0'; i ++)
				{
					OLED_ShowChar(X + i * Size, Page, String[i], Size);
				}	
}

void OLED_ShowNum(uint8_t X, uint8_t Page, uint32_t Num, uint8_t Length, uint8_t Size)	//��ʾ����
{
				for (uint8_t i = 0; i < Length; i ++)		//�������ֵ�ÿһλ							
				{
					/*����OLED_ShowChar������������ʾÿ������*/
					/*Number / OLED_Pow(10, Length - i - 1) % 10 ����ʮ������ȡ���ֵ�ÿһλ*/
					/*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
					OLED_ShowChar(X + i * Size, Page, Num / OLED_Pow(10, Length - i - 1) % 10 + '0', Size);
				}
}

void OLED_ShowSignedNum(uint8_t X, uint8_t Page, int32_t Num, uint8_t Length, uint8_t Size)	//��ʾ����������
{
				uint32_t Num1;
				
				if (Num >= 0)						//���ִ��ڵ���0
				{
					OLED_ShowChar(X, Page, '+', Size);	//��ʾ+��
					Num1 = Num;					//Number1ֱ�ӵ���Number
				}
				
				else									//����С��0
				{
					OLED_ShowChar(X, Page, '-', Size);	//��ʾ-��
					Num1 = -Num;					//Number1����Numberȡ��
				}
				
				for (uint8_t i = 0; i < Length; i++)			//�������ֵ�ÿһλ								
				{
					/*����OLED_ShowChar������������ʾÿ������*/
					/*Number1 / OLED_Pow(10, Length - i - 1) % 10 ����ʮ������ȡ���ֵ�ÿһλ*/
					/*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
					OLED_ShowChar(X + (i + 1) * Size, Page, Num1 / OLED_Pow(10, Length - i - 1) % 10 + '0', Size);
				}
}

void OLED_ShowFloatNum(uint8_t X, uint8_t Page, double Num, uint8_t IntLength, uint8_t FraLength, uint8_t Size)	//��ʾ������С��
{
				uint32_t PowNum, IntNum, FraNum;
				
				if (Num >= 0)						//���ִ��ڵ���0
				{
					OLED_ShowChar(X, Page, '+', Size);	//��ʾ+��
				}
				
				else									//����С��0
				{
					OLED_ShowChar(X, Page, '-', Size);	//��ʾ-��
					Num = -Num;					//Numberȡ��
				}
				
				/*��ȡ�������ֺ�С������*/
				IntNum = Num;						//ֱ�Ӹ�ֵ�����ͱ�������ȡ����
				Num -= IntNum;						//��Number��������������ֹ֮��С���˵�����ʱ����������ɴ���
				PowNum = OLED_Pow(10, FraLength);		//����ָ��С����λ����ȷ������
				FraNum = round(Num * PowNum);		//��С���˵�������ͬʱ�������룬������ʾ���
				IntNum += FraNum / PowNum;				//��������������˽�λ������Ҫ�ټӸ�����
				
				/*��ʾ��������*/
				OLED_ShowNum(X + Size, Page, IntNum, IntLength, Size);
				
				/*��ʾС����*/
				OLED_ShowChar(X + (IntLength + 1) * Size, Page, '.', Size);
				
				/*��ʾС������*/
				OLED_ShowNum(X + (IntLength + 2) * Size, Page, FraNum, FraLength, Size);
}

void OLED_ShowHexNum(uint8_t X, uint8_t Page, uint32_t Num, uint8_t Length, uint8_t Size)	//��ʾʮ��������
{
				uint8_t  SingleNum;
				for (uint8_t i = 0; i < Length; i ++)		//�������ֵ�ÿһλ
				{
					/*��ʮ��������ȡ���ֵ�ÿһλ*/
					SingleNum = Num / OLED_Pow(16, Length - i - 1) % 16;
					
					if (SingleNum < 10)			//��������С��10
					{
						/*����OLED_ShowChar��������ʾ������*/
						/*+ '0' �ɽ�����ת��Ϊ�ַ���ʽ*/
						OLED_ShowChar(X + i * Size, Page, SingleNum + '0', Size);
					}
					else							//�������ִ���10
					{
						/*����OLED_ShowChar��������ʾ������*/
						/*+ 'A' �ɽ�����ת��Ϊ��A��ʼ��ʮ�������ַ�*/
						OLED_ShowChar(X + i * Size, Page, SingleNum - 10 + 'A', Size);
					}
				}
}

void OLED_ShowImage(uint8_t X, uint8_t Page, uint8_t Width, uint8_t HeightByPage, const uint8_t* Image)	//��ʾͼ��
{
				for (uint8_t i = 0; i < HeightByPage; i ++)
				{
					OLED_SetCursor(X, Page + i);
				
					for (uint8_t j = 0; j < Width; j ++)
					{
						OLED_WrtData(Image[Width * i + j]);
					}
				}
}

void OLED_OpenShow(void)//��������
{
			OLED_Clear();
			OLED_ShowImage(0, 0, 64, 8, NWPU);
			Delay_ms(200);
			OLED_ShowImage(64, 0, 64, 4, xc);
			Delay_ms(200);
			OLED_ShowImage(64, 4, 64, 4, tk);
			Delay_ms(1600);
			OLED_Clear();
}
