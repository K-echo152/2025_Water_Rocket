#ifndef __MPU6050_H
#define __MPU6050_H

typedef struct 
{
			int16_t AX;
			int16_t AY;
			int16_t AZ;		//���ٶ�
			int16_t GX;
			int16_t GY;
			int16_t GZ;	//������
}	MPU6050_Data;

//�ڲ�����
//void MPU6050_WriteByte2Reg(uint8_t RegAddress, uint8_t Byte);
//uint8_t MPU6050_ReadByte2Reg(uint8_t RegAddress);

////void MPU6050_WriteData2Reg(uint8_t RegAddress, uint8_t Data);	//��δ���	
////uint8_t MPU6050_ReadData2Reg(uint8_t RegAddress);

void MPU6050_Init(void);	//��ʼ��
uint8_t MPU6050_ReadID(void);  //��ȡӲ��id
void MPU6050_GetData(MPU6050_Data* Data);	//��ȡ����������
float MPU6050_CaculateAccel(MPU6050_Data *Data);	//

#endif
