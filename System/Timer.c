#include "stm32f10x.h"

/*��ʱ��ʵ�ʾ��Ǽ�����������*/

void Timer_Internal_Init(void)
{
			//�ȿ���TIM��Ӧ��RCC�ڲ�ʱ��
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
			//ѡ��ʱ����Ԫ��ʱ�ӣ���ѡ��Ĭ���ڲ�
			//TIM_InternalClockConfig				//RCC�ڲ�ʱ��
			//TIM_ITRxExternalClockConfig		//ITRx������ʱ��
			//TIM_TIxExternalClockConfig		//TIx����ͨ��
			//TIM_ETRClockMode1Config
			//TIM_ETRClockMode2Config				//EXT�ⲿʱ��
			
			//ѡ���ڲ�ʱ������TIM��ʱ����Ԫ
			TIM_InternalClockConfig(TIM2);
	
			//����ʱ����Ԫ
			TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	//������ؽṹ��
			TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//ʱ�ӷ�Ƶ�����˲���أ�ѡ��1��Ƶ
			TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//������ģʽ�����������������ȣ�ѡ�����϶���
			TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//�ظ���������ֵ���߼���ʱ�����У�����
			
	//�ؼ���������ʱƵ��=72M/(PSC+1)/(ARR+1)������0-65535��
			TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;	//ARR�Զ���װ����ֵ
			TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;	//PSCԤ��Ƶ����ֵ
	//����õ�0.1s�ķֶȣ�����ϸ�����޸�
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
			//�ֶ������־λ������ճ�ʼ������ж�ʹ�ü�ʱ��0��ʼ
			TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
			//ʹ�ܸ����ж�(Update)��NVIC��ͨ·
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
			
			//NVIC���ȼ�����
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			
			//����NVIC�ж�
			NVIC_InitTypeDef NVIC_InitStructure;
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//�ж�ͨ��,ѡ��TIM��Ӧ��
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ��
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//��Ӧ���ȼ�
			NVIC_Init(&NVIC_InitStructure);
			
			//ʹ�ܣ���ʱ����ʱ�ж�
			TIM_Cmd(TIM2, ENABLE);
}

void Timer_External_Init(void)	//�ⲿ�ж϶�ʱ��(������)
{
			//���ڲ��жϵ�ģ��֮���޸ļ��ɣ��ظ���ע��ɾ��
	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
			//�ⲿҪ����GPIO����
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//�Ƽ����գ���������ѡ������
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//TIM��EXT���Ÿ���
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
	
			TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
			TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
			TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
			TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	
			TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
			
	//�ؼ���������ʱƵ��=72M/(PSC+1)/(ARR+1)������0-65535��
	//�ⲿ�����Զ������Ϊ����Ԥ��ƵΪ������CNT��һ
	//												����װΪCNT�����ٲ��ж�һ��
			TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;	//ARR�Զ���װ����ֵ
			TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;	//PSCԤ��Ƶ����ֵ

			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
			TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
			
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			
			NVIC_InitTypeDef NVIC_InitStructure;
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_Init(&NVIC_InitStructure);
			
			TIM_Cmd(TIM2, ENABLE);
}

/*��ʱ�ж�ֱ�Ӹ�������Ҫʹ�õĵط�����
void TIM2_IRQHandler(void)	//ѡ��TIM���жϴ����������������ж�ʱ���Զ�ִ��
{
			
			if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)	//�ȼ���жϱ�־λ����ȡTIM�ĸ����жϱ�־λ
			{
					//��д��Ӧ�ĺ���
					TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//�����־λ
			}
}
*/





