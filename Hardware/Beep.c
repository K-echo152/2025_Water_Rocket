#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*��Դ��������PWMƵ�ʼ�Ƶ��*/

void Beep_Init(void)
{			
			/*����Ҫ������ӳ�䣬ʹ���·�����*/
//			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//��AFIO
//			//��Ŀ��˿��ϵ�Ĭ���е��Զ˿ڣ�Ҫ��ȡ��
//			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//			GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);	//���Ѱ����ӳ���Ӧ����
	
	
	
			//����GPIO
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�������죬��Ϊ������Ȩת�������õ�PWM����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			//�ȿ���TIM��Ӧ��RCC�ڲ�ʱ��
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
			//ѡ��ʱ����Ԫ��ʱ�ӣ���ѡ��Ĭ���ڲ�
			//��RCC�ڲ�ʱ�ӡ�ITRx������ʱ����TIx����ͨ����EXT�ⲿʱ��
			//ѡ���ڲ�ʱ������TIM��ʱ����Ԫ
			TIM_InternalClockConfig(TIM4);
	
			//����ʱ����Ԫ
			TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	//������ؽṹ��
			
			TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//ʱ�ӷ�Ƶ�����˲���أ�ѡ��1��Ƶ
			TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//������ģʽ�����������������ȣ�ѡ�����϶���
			TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//�ظ���������ֵ���߼���ʱ�����У�����
	//�ؼ���������ʱƵ��=72M/(PSC+1)/(ARR+1)������0-65535��
			TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;	//ARR�Զ���װ����ֵ
			TIM_TimeBaseInitStructure.TIM_Prescaler = 1377 - 1;	//PSCԤ��Ƶ����ֵ
			TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
			//��������Ƚ�
			TIM_OCInitTypeDef TIM_OCInitStructure;
			TIM_OCStructInit(&TIM_OCInitStructure);	//û�и����г�Ա��ֵ����Ĭ�ϳ�ʼ��
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//����Ƚ�ģʽ
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//����Ƚϵļ���
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
			TIM_OCInitStructure.TIM_Pulse = 50;	//����CCR
	//���㹫ʽ��PWMƵ��=72M/(PSC+1)/(ARR+1)
	//					ռ�ձ�=CCR/(ARR+1)
	//					�ֱ���=1/(ARR+1)
	
			//TIM_OutputNState��TIM_OCIdleState��TIM_OCNIdleState��TIM_OCNPolarity//��Щ�߼���ʱ���Ż�ʹ�õ�
			
			TIM_OC2Init(TIM4, &TIM_OCInitStructure);	//����TIM��CH������
			/*ͬһTIM�Ķ��CH���Լ�����*/
			
			//TIM_CtrlPWMOutputs//���߼���ʱ��ʹ�ã���ʹ�ø߼���ʱ�����PWMʱ����ò�ʹ�ܲ������
			
			//ʹ�ܣ�����
			TIM_Cmd(TIM4, ENABLE);
}

void Beep_Ring(uint16_t PSC)	//�ڳ��������иı�PSC���ı�Ƶ�ʼ�����
{
			TIM_SetCompare2(TIM4, 50);
			TIM_PrescalerConfig(TIM4, PSC, TIM_PSCReloadMode_Update);
}

void Beep_Stop(void)	//ֹͣ
{
			TIM_SetCompare2(TIM4, 0);
}

void Beep_JustLoud(void)	//����������2700Hz
{
			TIM_SetCompare2(TIM4, 50);
			TIM_PrescalerConfig(TIM4, 266, TIM_PSCReloadMode_Update);
}


/*�������*/
//uint16_t Beep_C_low[7] = {};
const uint16_t Beep_C_mid[8] = {1377, 1227, 1093, 1032, 918, 818, 729, 688};
//uint16_t Beep_C_Hih[7] = {};









