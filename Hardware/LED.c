#include "stm32f10x.h"

void LED13_Init(void)
{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
	
			BKP_TamperPinCmd(DISABLE);	//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
			BKP_ITConfig(DISABLE);
	
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);	//PC13λ�͵�ƽ����
}

void LED13_on(void)
{
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LED13_off(void)
{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

