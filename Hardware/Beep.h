#ifndef __BEEP_H
#define __BEEP_H

void Beep_Init(void);
void Beep_Ring(uint16_t PSC);	//�ڳ��������иı�PSC���ı�Ƶ�ʼ�����
void Beep_Stop(void);	//ֹͣ
void Beep_JustLoud(void);	//����������2700Hz
	
extern const uint16_t Beep_C_mid[8];

#endif
