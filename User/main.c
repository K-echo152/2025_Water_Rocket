#include "stm32f10x.h"
#include "LED.h"
#include <math.h>
#include "Beep.h"
#include "Timer.h"
#include "Delay.h"
#include "Servo.h"
#include "BMP280.h"
#include "W25Q64.h"
#include "MPU6050.h"
#include "ShortLine.h"
#include "OLED_Simple.h"


int16_t parheight, plaheight, begheight, mostheight, nowheight;
uint8_t height[2];
MPU6050_Data Data;
uint16_t Height;

int main(void) // Ӳ������
{
		LED13_Init();
		Beep_Init();
		OLED_Init();
		MPU6050_Init();
		BMP280_Init();
		
		OLED_OpenShow();
	
		OLED_ShowString(0, 0,"BMPID:",BIG);
		OLED_ShowString(0, 2,"MPUID:",BIG);
	    OLED_ShowString(0, 6,"Accel:",BIG);
		OLED_ShowString(0, 4,"Height:",BIG);
	
		OLED_ShowHexNum(48, 0, BMP280_ReadID(), 2, BIG);
		OLED_ShowHexNum(48, 2, MPU6050_ReadID(), 2, BIG);
	
	

		while(1)
		{
			LED13_on();
			Beep_JustLoud();
			Delay_ms(100);
			LED13_off();
			Beep_Stop();
			MPU6050_GetData(&Data);
//			OLED_ShowFloatNum(48, 6, MPU6050_CaculateAccel(&Data), 2,2, BIG);
			Height = BMP280_GetHeight();
			OLED_ShowFloatNum(56, 4, BMP280_GetHeight(), 3, 2, BIG);
			Delay_ms(100);
		}
	
}


//int main(void)
// {
//			{/*//�����ø߶ȼƴ��벿��
//			OLED_Init(); OLED_OpenShow();	//��������
//			BMP280_Init();
//			W25Q64_Init();
//			Beep_Init();
//	 
//			OLED_ShowString(0, 0,"last:", BIG);
//			W25Q64_ReadData(0x000000, height, 2);
//			OLED_ShowNum(0, 2, (height[0] << 8) + height[1], 3, BIG);
//			Delay_s(10);
//			
//			Beep_JustLoud();
//			Delay_ms(750);
//			Beep_Stop();
//			Delay_ms(1000);
//			Beep_JustLoud();
//			Delay_ms(750);
//			Beep_Stop();
//			Delay_ms(1000);
//			Beep_JustLoud();
//			Delay_ms(750);
//			Beep_Stop();
//			
//			OLED_Clear();
//			
//			OLED_ShowString(0, 0, "now:", BIG);
//			OLED_ShowString(48, 0, "beg:", BIG);
//			OLED_ShowString(96, 0, "mst:", BIG);
//	
//			beginheight = BMP280_GetHeight();
//			OLED_ShowNum(48, 2, beginheight, 3, BIG);
//			OLED_ShowChar(72, 2, 'm', BIG);
//	
//			mostheight = beginheight;
//			OLED_ShowNum(96, 2, mostheight, 3, BIG);
//			OLED_ShowChar(120, 2, 'm', BIG);
//	
//			OLED_ShowChar(24, 2, 'm', BIG);
//			
//			while (1)
//			{
//			/////////////////////////////////////////////////////////////////////////////////////�߶���ʾ
//			nowheight=BMP280_GetHeight();
//			OLED_ShowNum(0, 2, nowheight, 3, BIG);

//			if (nowheight > mostheight)
//			{
//					mostheight = nowheight;
//					height[0] = mostheight >> 8;
//					height[1] = mostheight;
//					W25Q64_SectorErase(0x000000);
//					W25Q64_PageProgram(0x000000, height, 2);
//			}
//			
//			OLED_ShowNum(96, 2, mostheight, 3, BIG);
//			}
//			*/}
//			
//		{//ʵ�ʷɿز���
//			
//			/*---------------------------���Ӳ����ʼ��---------------------------*/
//		
//			ShortLine_Init();	//
//			LED13_Init();
//			Beep_Init();
//			Servo_Init();
//			BMP280_Init();
//			OLED_Init();
//			W25Q64_Init();
//			MPU6050_Init();
//				
//			/*------------------------------��ǹ���------------------------------*/
//			
//			Servo1_SetAngle(0);
//			Servo2_SetAngle(0);	
//			
//			/*------------------------------OLED����------------------------------*/
//			
//			OLED_OpenShow();	
//			
//			/*----------------------------��ʾ�ϴ�����----------------------------*/
//			
//			OLED_ShowString(0, 0,"last:", BIG);
//			W25Q64_ReadData(0x000000, height, 2);
//			OLED_ShowNum(0, 2, (height[0] << 8) + height[1], 3, BIG);
//			Delay_s(3);
//			OLED_Clear();
//			
//			/*----------------------------������ʾ׼��----------------------------*/
//			
//			OLED_ShowString(0, 0, "BEGa&h:", BIG);
//			OLED_ShowString(0, 2, "PLAN:", BIG);
//			OLED_ShowString(0, 4, "PARA:", BIG);
//			OLED_ShowString(0, 6, "MOST:", BIG);
//			
//				
//			/*---------------------��һ�γ��������̽������---------------------*/
//			
//			Beep_JustLoud();	
//			LED13_on();
//			Delay_ms(500);
//			while(ShortLine() == 0)	//δ�Ӷ̽���һֱ����
//			{
//					
//			}
//				
//			Beep_Stop();	//ֱ������ֹͣ
//			LED13_off();
//			
//			/*-----------------------�ڶ��ζ�����׼���÷���-----------------------*/
//			
//			while(ShortLine())
//			{
//					Beep_Stop();
//					Delay_ms(200);
//					LED13_on();
//					Beep_JustLoud();
//					Delay_ms(200);
//					LED13_off();
//					
//					if(ShortLine() == 0)
//					{
//							Delay_ms(50);	//�ʶ���ʱ����ֹ����̽���ʱ����
//					}	
//			}
//				
//			/*-------------------------------�����-------------------------------*/
//			
//			MPU6050_GetData(&Data);	//��ȡ����ʾ��ܼ��ٶ�����
//			OLED_ShowFloatNum(54, 0, MPU6050_CaculateAccel(&Data), 2, 3, SML); 
//			
//			OLED_ShowNum(54, 1, mostheight = BMP280_GetHeight(), 3, SML);	//��ʾ��ʼ�߶�
//			
//			for(int i = 0; i < 7; i ++)	//�׷ɻ�
//			{
//					LED13_on();
//					Delay_ms(500);
//					LED13_off();
//					nowheight = BMP280_GetHeight();
//					if(nowheight > mostheight)
//					{
//							OLED_ShowNum(48, 6, mostheight = nowheight, 3, BIG);
//					}
//			}
//			
//			Servo1_SetAngle(90);
//			OLED_ShowNum(48, 2, BMP280_GetHeight(), 3, BIG);	//��ʾ�׷ɻ��߶�
//			
//			
//			Delay_ms(500);	//��ɡ
//			Servo2_SetAngle(90);
//			OLED_ShowNum(48, 4, nowheight = BMP280_GetHeight(), 3, BIG);	//��ʾ��ɡ�߶�
//			
//			/*---------------------��ɺ��¼���߶ȱ��ڸĽ�---------------------*/
//			
//			Beep_Stop();
//			OLED_ShowNum(48, 6, mostheight, 3, BIG);
//			
//			while(1)
//			{
//					nowheight = BMP280_GetHeight();
//				
//					if(nowheight > mostheight)
//					{	
//							OLED_ShowNum(48, 6, mostheight = nowheight, 3, BIG);
//						
//							height[0] = mostheight >> 8;
//							height[1] = mostheight;
//							W25Q64_SectorErase(0x000000);
//							W25Q64_PageProgram(0x000000, height, 2);
//					}
//			}                                     
//			
//		}
//			                            
// }
//	                                            
                                                                                                                                                                                                                                                                   