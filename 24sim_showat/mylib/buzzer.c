#include "buzzer.h"
#include "bitband.h"

void buzzer_init(void)
{
	GPIO_InitTypeDef Buzzer_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//ͨ��APB2����ʹ��GPIOC���ʱ��
	
	Buzzer_Value.GPIO_Mode = GPIO_Mode_Out_PP;//��������
	Buzzer_Value.GPIO_Pin = GPIO_Pin_7;//����7�Źܽ�
	Buzzer_Value.GPIO_Speed = GPIO_Speed_50MHz;//����Ƶ��
	GPIO_Init(GPIOC, &Buzzer_Value);
	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_7);//�µ͵�ƽ
	PCOut(7) = 0;
}

void buzzer_on(void)
{
//	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	PCOut(7) = 1;
}

void buzzer_off(void)
{
//	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	PCOut(7) = 0;
}







