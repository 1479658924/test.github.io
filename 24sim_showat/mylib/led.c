#include "led.h"
#include "bitband.h"


void led_init(void)//初始化的函数(初始化LED灯所连接的GPIO管脚)
{
	GPIO_InitTypeDef Led_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
	
	Led_Value.GPIO_Mode = GPIO_Mode_Out_PP;//设置推挽
	Led_Value.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//设置管脚
	Led_Value.GPIO_Speed = GPIO_Speed_50MHz;//设置频率
	GPIO_Init(GPIOC, &Led_Value);//初始化管教
	
	//GPIO_ResetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);//设置低电平
	PCOut(1) = 0;
	PCOut(2) = 0;
	PCOut(3) = 0;//把管脚拉低
}

void led_on(int nu)//根据nu参数点亮相应的LED灯
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

void led_off(int nu)//根据nu参数灭掉相应的LED灯
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




