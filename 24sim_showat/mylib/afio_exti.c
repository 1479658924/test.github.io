#include "afio_exti.h"

handler exti_h0, exti_h1, exti_h2;

void exti_init(void)
{
	GPIO_InitTypeDef Gpio_Value;
	NVIC_InitTypeDef Nvic_Value;
	EXTI_InitTypeDef Exti_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |\
			RCC_APB2Periph_AFIO, ENABLE);
	//����PC8 9 PA0������ģʽ
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_Init(GPIOC, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPD;
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	//�����ⲿ�жϴ�������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	//�������ȼ�  ��ռ/ռ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���������Ⱥ������ȼ�
	Nvic_Value.NVIC_IRQChannel = EXTI9_5_IRQn;//�жϺ�ѡ��
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//���������ȼ�����Ϊ2
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//���������ȼ�����Ϊ2
	NVIC_Init(&Nvic_Value);
	
	Nvic_Value.NVIC_IRQChannel = EXTI0_IRQn;//0���жϺ�
	NVIC_Init(&Nvic_Value);

	//ѡ���ⲿ�жϺ� ������ʽ �ж�ģʽ
	Exti_Value.EXTI_Line = EXTI_Line9 | EXTI_Line8;
	Exti_Value.EXTI_LineCmd = ENABLE;
	Exti_Value.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_Init(&Exti_Value);
	
	Exti_Value.EXTI_Line = EXTI_Line0;
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_Init(&Exti_Value);
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line9) == SET)
	{
		exti_h0();
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	if (EXTI_GetITStatus(EXTI_Line8) == SET)
	{
		exti_h1();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

void EXTI0_IRQHandler(void)
{
	exti_h2();
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void exti_handler(handler h1, handler h2, handler h3)
{
	exti_h0 = h1;
	exti_h1 = h2;
	exti_h2 = h3;
}









