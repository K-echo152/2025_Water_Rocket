#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/*�����Э��涨��PWM����ҪΪ20ms���ߵ�ƽ0.5msΪ0�㣬�ߵ�ƽ2.5msΪ180��*/
/*����Э������PWM�Ĳ���������ARR+1=20000��PSC+1=72��CCR��ʼΪ0*/
/*���ݼ�����У�Ƶ��=50(���ڼ�Ϊ20ms)��ռ�ձ�=CCR/20000���ֱ���=1/20000*/


void Servo_Init(void)	//ʹ��ǰ����PWM��Ҫ�����
{
			PWM_Init();
}

void Servo1_SetAngle(float Angle)
{
			//�Ƕ���0-180����Э��CCRӦ��500-2500���л����ϵ��
			PWM_SetCompare2(Angle / 180 *2000 + 500);
}

void Servo2_SetAngle(float Angle)
{
			//�Ƕ���0-180����Э��CCRӦ��500-2500���л����ϵ��
			PWM_SetCompare3(Angle / 180 *2000 + 500);
}

void Servo3_SetAngle(float Angle)
{
			//�Ƕ���0-180����Э��CCRӦ��500-2500���л����ϵ��
			PWM_SetCompare1(Angle / 180 *2000 + 500);
}
