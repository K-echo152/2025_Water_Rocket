#ifndef __LEOSPI_H
#define __LEOSPI_H

/*�ڲ�����*/
//void leoSPI_W_SS(uint8_t BitValue);	//дSS���������ӻ�ѡ�����
//void leoSPI_W_SCK(uint8_t BitValue);	//дSCK������������ʱ�ӣ����
//void leoSPI_W_MOSI(uint8_t BitValue);	//дMOSI��������������ݣ����
//uint8_t leoSPI_R_MISO(void);	//��MISO���������������ݣ�����

void leoSPI_Init(void);	//SPI��ʼ��
void leoSPI_Start(void);	//��ʼ������SS�Ӹߵ�ƽ�õ͵�ƽ
void leoSPI_Stop(void);	//��ֹ������SS�ӵ͵�ƽ�øߵ�ƽ
uint8_t leoSPI_SwapByte(uint8_t ByteSend);	//ģʽ0�����ֽڣ�������ģʽҪ��Ӧ����

#endif
