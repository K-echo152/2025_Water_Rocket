#ifndef __LEOI2C_H
#define __LEOI2C_H

//�ڲ�����
//void leoI2C_W_SCL(uint8_t BitValue);	//дʱ����
//void leoI2C_W_SDA(uint8_t BitValue);	//д������
//uint8_t	leoI2C_R_SDA(void);	//��������

//��������
void leoI2C_Init(void);	//I2C��ʼ��

void leoI2C_Start(void);	//��ʼ����
void leoI2C_Stop(void);	//��ֹ����
void leoI2C_SendByte(uint8_t Byte);	//����һ���ֽ�
uint8_t leoI2C_ReceiveByte(void);	//����һ���ֽ�
void leoI2C_SendAck(uint8_t AckBit);	//����Ӧ��
uint8_t leoI2C_ReceiveAck(void);	//����Ӧ��


#endif
