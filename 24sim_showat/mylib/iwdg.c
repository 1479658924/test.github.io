#include "iwdg.h"
/*
void iwdg_init(int ms)//看门狗初始化
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//开启写使能
	IWDG_SetPrescaler(IWDG_Prescaler_64);//设置64分频 625Hz
	IWDG_SetReload(ms);//设置时间
	IWDG_ReloadCounter();//初始喂狗
	IWDG_Enable();//启动看门狗
}

void iwdg_feed(void)//喂狗
{
	IWDG_ReloadCounter();
}
*/
//独立看门狗寄存器 0x40003000
void iwdg_init(int ms)//看门狗初始化
{
	
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0x5555;//开启写使能
	*(volatile unsigned int *)(0x40003000 + 0x04) = 0x06;//设置256分频 156.25Hz
	*(volatile unsigned int *)(0x40003000 + 0x08) = ms;//设置时间
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0xAAAA;//初始喂狗
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0xCCCC;//启动看门狗
}

void iwdg_feed(void)//喂狗
{
	*(volatile unsigned int *)(0x40003000 + 0x00) = 0xAAAA;
}










