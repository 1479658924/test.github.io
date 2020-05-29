#include "iwdg.h"
/*
void iwdg_init(int ms)//���Ź���ʼ��
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//����дʹ��
	IWDG_SetPrescaler(IWDG_Prescaler_64);//����64��Ƶ 625Hz
	IWDG_SetReload(ms);//����ʱ��
	IWDG_ReloadCounter();//��ʼι��
	IWDG_Enable();//�������Ź�
}

void iwdg_feed(void)//ι��
{
	IWDG_ReloadCounter();
}
*/
//�������Ź��Ĵ��� 0x40003000
void iwdg_init(int ms)//���Ź���ʼ��
{
	
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0x5555;//����дʹ��
	*(volatile unsigned int *)(0x40003000 + 0x04) = 0x06;//����256��Ƶ 156.25Hz
	*(volatile unsigned int *)(0x40003000 + 0x08) = ms;//����ʱ��
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0xAAAA;//��ʼι��
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0xCCCC;//�������Ź�
}

void iwdg_feed(void)//ι��
{
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0xAAAA;
}










