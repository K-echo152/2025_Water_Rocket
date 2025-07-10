#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/*PWM����ֱ���������Ҫ��������оƬ��Ҫ������GPIO�ڿ��Ʒ�����һ��PWM�ڿ����ٶ�*/
/*PWM������ARR+1=100��PSC+1=36(����Ƶ�����˶���Χ��)��CCR��0-100����ת��*/

void Motor_Init(void)
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIO��ʼ�������Ʒ���
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			PWM_Init();	//PWM��ʼ��������
}

void Motor_SetSpeed(int8_t Speed)	//�����ٶȣ�-100��+100
{
			if (Speed >= 0)
			{
					GPIO_SetBits(GPIOA, GPIO_Pin_4);
					GPIO_ResetBits(GPIOA, GPIO_Pin_5);
					PWM_SetCompare1(Speed);
			}
			
			else
			{
					GPIO_SetBits(GPIOA, GPIO_Pin_5);
					GPIO_ResetBits(GPIOA, GPIO_Pin_4);
					PWM_SetCompare1(-1 * Speed);
			}
}


