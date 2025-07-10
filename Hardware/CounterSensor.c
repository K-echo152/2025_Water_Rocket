#include "stm32f10x.h"                  // Device header

/*����ʽ���⴫�мƴΣ���Ҫ�����ⲿ�жϵ�ѧϰ*/

uint16_t CounterSensor_Count;

void CounterSensor_Init(void)
{
			//�򿪶�Ӧ����ʱ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIO����
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//AFIO���裬�����ض������ⲿ�ж�
			//NVIC�ں����費��Ҫ����ʱ��
			//EXTIδ֪
	
			//����GPIO
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			//����AFIO
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
			//����EXTI
			EXTI_InitTypeDef EXTI_InitStructure;
			EXTI_InitStructure.EXTI_Line = EXTI_Line14;	//ѡ������Ŷ�Ӧ�ж���
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;	//������ѡ��ENABLE
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�ж�or�¼�
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//���ش���
			EXTI_Init(&EXTI_InitStructure);
	
			//����NVIC
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//NVIC�ж����ȼ����䣬һ������ֻ��һ��
			NVIC_InitTypeDef NVIC_InitStructure;
			NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//ָ���ж�ͨ���������ж��߶�Ӧ
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//��Ӧ���ȼ�
			NVIC_Init(&NVIC_InitStructure);	
}

uint16_t CounterSensor_Get(void)
{
			return CounterSensor_Count;
}

void EXTI15_10_IRQHandler(void)	//�жϴ��������ǹ̶���
{
			//10-15���ܽ��������ж��ǲ�����Ҫ��������
			if (EXTI_GetITStatus(EXTI_Line14) == SET)
			{
					/*��д������*/
					CounterSensor_Count ++;
					EXTI_ClearITPendingBit(EXTI_Line14);
			}
}
