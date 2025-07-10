#ifndef __BMP280_H
#define __BMP280_H

/*�ڲ�����*/
//void BMP280_WriteByte2Reg(uint8_t RegAddress, uint8_t Byte);
//uint8_t BMP280_ReadByte2Reg(uint8_t RegAddress);

/*�м亯��*/
//void BMP280_WaitRead(void);	//�ȴ�ת������Զ�ȡ����
//double BMP280_TempCorrect(int32_t adc_T);	//�������϶�
//double BMP280_PresCorrect(int32_t adc_P);	//����Pa
	

void BMP280_Init(void);	//BMP280��ʼ��
//double BMP280_GetTemperature(void);	//��ȡ�Ĵ����ڵ�ԭʼ�¶����ݲ�����ת��Ϊ׼ȷֵ
void BMP280_GetData(double *Temp, double *Pres);	//��ȡ�Ĵ����ڵ�ԭʼ��ѹ���ݲ�����ת��Ϊ׼ȷֵ
uint8_t BMP280_ReadID(void);
uint16_t BMP280_GetHeight(void);

typedef struct  
{
	/* T1~P9 Ϊ����ϵ�� */
	uint16_t T1;
	int16_t	T2;
	int16_t	T3;
	uint16_t P1;
	int16_t	P2;
	int16_t	P3;
	int16_t	P4;
	int16_t	P5;
	int16_t	P6;
	int16_t	P7;
	int16_t	P8;
	int16_t	P9;
} BMP280;

#endif
