#include "buzzer.h"
#include "bitband.h"

void buzzer_init(void)
{
	GPIO_InitTypeDef Buzzer_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
	
	Buzzer_Value.GPIO_Mode = GPIO_Mode_Out_PP;//设置推挽
	Buzzer_Value.GPIO_Pin = GPIO_Pin_7;//设置7号管教
	Buzzer_Value.GPIO_Speed = GPIO_Speed_50MHz;//设置频率
	GPIO_Init(GPIOC, &Buzzer_Value);
	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_7);//致低电平
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







