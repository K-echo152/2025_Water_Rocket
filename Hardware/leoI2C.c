#include "stm32f10x.h"
#include "Delay.h"


/*�˴�Ϊ���I2C���������I2C�⺯����ֻ����GPIO����ʵ��I2C
	��Ҫ�޸Ķ�Ӧ��ʱ������������ֻ�����ǰ�ĸ�����
	��ͬʱ�õ��˶��ʹ��I2Cͨ�ŵ��豸
	���Էֱ�д���Ӳ�������������Ҳ��������ͬһ��I2C���ϣ�ΪI2C����������*/
	
#define leoI2C_SCL_GPIO GPIOB
#define leoI2C_SCL_PIN GPIO_Pin_8
#define leoI2C_SDA_GPIO GPIOB
#define leoI2C_SDA_PIN GPIO_Pin_9


//�Զ�д���к�����װ
void leoI2C_W_SCL(uint8_t BitValue)	//дʱ����
{
			GPIO_WriteBit(leoI2C_SCL_GPIO, leoI2C_SCL_PIN,	(BitAction)BitValue);
			Delay_us(1);//�����Ƶ��ֲ
}

void leoI2C_W_SDA(uint8_t BitValue)	//д������
{
			GPIO_WriteBit(leoI2C_SDA_GPIO, leoI2C_SDA_PIN,	(BitAction)BitValue);
			Delay_us(1);
}

uint8_t	leoI2C_R_SDA(void)	//��������
{
			uint8_t BitValue;
			BitValue = GPIO_ReadInputDataBit(leoI2C_SDA_GPIO, leoI2C_SDA_PIN);
			Delay_us(1);
			return BitValue;
}

void leoI2C_Init(void)	//I2C��ʼ��
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��SCL��SDA��ʼ��Ϊ��©���ģʽ
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
			GPIO_InitStructure.GPIO_Pin = leoI2C_SCL_PIN;
			GPIO_Init(leoI2C_SCL_GPIO, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = leoI2C_SDA_PIN;
			GPIO_Init(leoI2C_SDA_GPIO, &GPIO_InitStructure);
	
			GPIO_SetBits(leoI2C_SCL_GPIO, leoI2C_SCL_PIN);
			GPIO_SetBits(leoI2C_SDA_GPIO, leoI2C_SDA_PIN);
}

//��I2C��Ӧ��6��ʱ�������Ԫ��Ӧ
//SCLʵ�����Ƕ�д����ı��֣�SCLΪ��ʱ������������д��SCLΪ��ʱ�������������
//���ߵ�ƽʱ���ݱ��޸ģ��������Ե�Ϊ��ʼ����ֹ

void leoI2C_Start(void)
{
			//��ʼ������SCL��SDA�ߵ�ƽʱ��SDA�Ӹߵ�ƽ�л����͵�ƽ,SCL�ٱ�Ϊ�͵�ƽ
			leoI2C_W_SDA(1);//���ͷ�SDA�Ƚϱ��գ�Ϊ�����ظ���ʼ��������SCLΪ�͵�ƽ��SDA��ȷ����
											//��SDAҲΪ��ʱ�����ͷ�SCL�����ͷ�SDAΪ��ֹ���������»���
	
			leoI2C_W_SCL(1);
			leoI2C_W_SDA(0);
			leoI2C_W_SCL(0);
}

void leoI2C_Stop(void)
{
			//��ֹ������SCL��SDA�͵�ƽʱ��SCL�ӵ͵�ƽ�л����ߵ�ƽ,SDA�ٱ�Ϊ�ߵ�ƽ
			leoI2C_W_SDA(0);//������SDA�Ƚϱ��գ���ȷ������������
			leoI2C_W_SCL(1);
			leoI2C_W_SDA(1);
}

void leoI2C_SendByte(uint8_t Byte)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬��������λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ����ӻ���SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			
			/*��������־�⣬SCL����0���������Կ�ʼ������SCLΪ0*/
			
			for(uint8_t i = 0; i < 8; i ++)
			{
				leoI2C_W_SDA(Byte & (0x80 >> i));
				leoI2C_W_SCL(1);
				leoI2C_W_SCL(0);	//���ص��ĸ����ϣ�����������ʱ
			}
}

uint8_t leoI2C_ReceiveByte(void)
{
			//����һ���ֽڣ�SCL�͵�ƽ�ڼ䣬�ӻ�����λ��λ���У����η���SDA���ϣ�Ȼ���ͷ�SCL
			//							SCL��λʱ����SDA�ȶ���������SDA��ȡ���ݣ������ٴ�����SCL���ظ�8��
			//							������Ҫ���գ�����ÿ�ζ�ȡǰҪ�ͷ�SDA��������Ȩ�����ӻ�
			
			uint8_t Byte = 0x00;
	
			leoI2C_W_SDA(1);
			for(uint8_t i = 0; i < 8; i ++)
			{
				
				leoI2C_W_SCL(1);
				if(leoI2C_R_SDA()) Byte |= (0x80 >> i);
				leoI2C_W_SCL(0);
			}
			
			return Byte;
}

void leoI2C_SendAck(uint8_t AckBit)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӷ���һλ����
			//					0��ʾ�յ���1��ʾδ�յ�
			leoI2C_W_SDA(AckBit);	
			leoI2C_W_SCL(1);
			leoI2C_W_SCL(0);
}

uint8_t leoI2C_ReceiveAck(void)
{
			//����Ӧ������������һ���ֽں�����һ��ʱ�ӽ���һλ����
			//					0��ʾ�ӻ��յ���1��ʾδ�յ�
			//					����ǰ���ͷ�SDA������Ȩ���ӻ�
			
			uint8_t AckBit;
			leoI2C_W_SDA(1);
			leoI2C_W_SCL(1);
			AckBit = leoI2C_R_SDA();
			leoI2C_W_SCL(0);
	
			return AckBit;
}
