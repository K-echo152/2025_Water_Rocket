#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{			
			/*����Ҫ������ӳ�䣬ʹ���·�����*/
//			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//��AFIO
//			//��Ŀ��˿��ϵ�Ĭ���е��Զ˿ڣ�Ҫ��ȡ��
//			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//			GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);	//���Ѱ����ӳ���Ӧ����
	
	
	
			//����GPIO
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�������죬��Ϊ������Ȩת�������õ�PWM����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_8;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			//�ȿ���TIM��Ӧ��RCC�ڲ�ʱ��
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
			//ѡ��ʱ����Ԫ��ʱ�ӣ���ѡ��Ĭ���ڲ�
			//��RCC�ڲ�ʱ�ӡ�ITRx������ʱ����TIx����ͨ����EXT�ⲿʱ��
			//ѡ���ڲ�ʱ������TIM��ʱ����Ԫ
			TIM_InternalClockConfig(TIM1);
			TIM_InternalClockConfig(TIM2);
			TIM_InternalClockConfig(TIM3);
	
			//����ʱ����Ԫ
			TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	//������ؽṹ��
			
			TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//ʱ�ӷ�Ƶ�����˲���أ�ѡ��1��Ƶ
			TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//������ģʽ�����������������ȣ�ѡ�����϶���
			TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//�ظ���������ֵ���߼���ʱ�����У�����
	//�ؼ���������ʱƵ��=72M/(PSC+1)/(ARR+1)������0-65535��
			TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;	//ARR�Զ���װ����ֵ
			TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;	//PSCԤ��Ƶ����ֵ
			TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	
			//��������Ƚ�
			TIM_OCInitTypeDef TIM_OCInitStructure;
			TIM_OCStructInit(&TIM_OCInitStructure);	//û�и����г�Ա��ֵ����Ĭ�ϳ�ʼ��
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//����Ƚ�ģʽ
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//����Ƚϵļ���
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
			TIM_OCInitStructure.TIM_Pulse = 0;	//����CCR
	//���㹫ʽ��PWMƵ��=72M/(PSC+1)/(ARR+1)
	//					ռ�ձ�=CCR/(ARR+1)
	//					�ֱ���=1/(ARR+1)
	
			//TIM_OutputNState��TIM_OCIdleState��TIM_OCNIdleState��TIM_OCNPolarity//��Щ�߼���ʱ���Ż�ʹ�õ�
			
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);	//����TIM��CH������
			TIM_OC1Init(TIM1, &TIM_OCInitStructure);
			TIM_OC3Init(TIM3, &TIM_OCInitStructure);
			/*ͬһTIM�Ķ��CH���Լ�����*/
			
			//TIM_CtrlPWMOutputs//���߼���ʱ��ʹ�ã���ʹ�ø߼���ʱ�����PWMʱ����ò�ʹ�ܲ������
			
			//ʹ�ܣ�����
			TIM_Cmd(TIM1, ENABLE);
			TIM_Cmd(TIM2, ENABLE);
			TIM_Cmd(TIM3, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)	//�ڳ����������޸�CH1��CCR���Դ��޸�ռ�ձ�
{
			TIM_SetCompare1(TIM1, Compare);
}

void PWM_SetCompare2(uint16_t Compare)	
{
			TIM_SetCompare2(TIM2, Compare);
}

void PWM_SetCompare3(uint16_t Compare)	
{
			TIM_SetCompare3(TIM3, Compare);
}

