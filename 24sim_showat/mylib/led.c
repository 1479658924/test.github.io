#include "led.h"
#include "bitband.h"


void led_init(void)//��ʼ���ĺ���(��ʼ��LED�������ӵ�GPIO�ܽ�)
{
	GPIO_InitTypeDef Led_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//ͨ��APB2����ʹ��GPIOC���ʱ��
	
	Led_Value.GPIO_Mode = GPIO_Mode_Out_PP;//��������
	Led_Value.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//���ùܽ�
	Led_Value.GPIO_Speed = GPIO_Speed_50MHz;//����Ƶ��
	GPIO_Init(GPIOC, &Led_Value);//��ʼ���ܽ�
	
	//GPIO_ResetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);//���õ͵�ƽ
	PCOut(1) = 0;
	PCOut(2) = 0;
	PCOut(3) = 0;//�ѹܽ�����
}

void led_on(int nu)//����nu����������Ӧ��LED��
{
	switch(nu)
	{
		/*
		case 0 : GPIO_SetBits(GPIOC, GPIO_Pin_1); break;
		case 1 : GPIO_SetBits(GPIOC, GPIO_Pin_2); break;
		case 2 : GPIO_SetBits(GPIOC, GPIO_Pin_3); break;
		*/
		case 0:	PCOut(1) = 1; break;
		case 1:	PCOut(2) = 1; break;
		case 2:	PCOut(3) = 1; break;
	}
}

void led_off(int nu)//����nu���������Ӧ��LED��
{
	switch(nu)
	{
		/*
		case 0 : GPIO_ResetBits(GPIOC, GPIO_Pin_1); break;
		case 1 : GPIO_ResetBits(GPIOC, GPIO_Pin_2); break;
		case 2 : GPIO_ResetBits(GPIOC, GPIO_Pin_3); break;
		*/
		case 0:	PCOut(1) = 0; break;
		case 1:	PCOut(2) = 0; break;
		case 2:	PCOut(3) = 0; break;		
	}
}


void led_on_all(void)
{
	int i = 0;
	for (i = 1; i <= 3; i ++)
		PCOut(i) = 1;
}

void led_off_all(void)
{
	int i = 0;
	for (i = 1; i <= 3; i ++)
		PCOut(i) = 0;
}	




