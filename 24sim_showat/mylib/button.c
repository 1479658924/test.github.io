#include "button.h"
#include "bitband.h"


//按键初始化
void button_init(void)
{
	GPIO_InitTypeDef Button_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	//通过APB2总线使GPIOC时钟使能
	
	Button_Value.GPIO_Mode = GPIO_Mode_IPU;//设置上拉输入
	Button_Value.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;//8、9管脚
	GPIO_Init(GPIOC, &Button_Value); // 管脚初始化
	
	Button_Value.GPIO_Mode = GPIO_Mode_IPD;//设置下拉输入
	Button_Value.GPIO_Pin = GPIO_Pin_0;//0号管脚
	GPIO_Init(GPIOA, &Button_Value);
}


//检测按下按键
int button_switch(int button_num)
{
	unsigned int ret = 0;
	switch (button_num) {
		case 0: 
//			ret = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
			ret = PCIn(9);
			break;
		case 1: 
//			ret = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
			ret = PCIn(8);
			break;
		case 2:
//			ret = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
			ret = PAIn(0);
			ret = !ret;
			break;
	}
	return !ret;
}











