#include "OLED_Data.h"

#ifndef __OLED_SIMPLE_H
#define __OLED_SIMPLE_H

//�궨��
#define BIG				8
#define SML				6

//�ڲ�����
//void OLED_WrtCmd(uint8_t Cmd);	//д����
//void OLED_WrtData(uint8_t Data);	//д����
//uint32_t OLED_Pow(uint32_t X, uint32_t Y);	

//��������
void OLED_Init(void);	//��ʼ��
void OLED_Clear(void);
void OLED_SetCursor(uint8_t X, uint8_t Page);	//���ù��

//���ú���
void OLED_ShowChar(uint8_t X, uint8_t Page, char Char, uint8_t Size);
void OLED_ShowString(uint8_t X, uint8_t Page, char* String, uint8_t Size);
void OLED_ShowNum(uint8_t X, uint8_t Page, uint32_t Num, uint8_t Length, uint8_t Size);	//��ʾ����
void OLED_ShowSignedNum(uint8_t X, uint8_t Page, int32_t Num, uint8_t Length, uint8_t Size);	//��ʾ����������
void OLED_ShowFloatNum(uint8_t X, uint8_t Page, double Num, uint8_t IntLength, uint8_t FraLength, uint8_t Size);	//��ʾ������С��
void OLED_ShowHexNum(uint8_t X, uint8_t Page, uint32_t Num, uint8_t Length, uint8_t Size);	//��ʾʮ��������
void OLED_ShowImage(uint8_t X, uint8_t Page, uint8_t Width, uint8_t HeightByPage, const uint8_t* Image);	//��ʾͼ��
void OLED_OpenShow(void);	//��������

#endif
