#include "button.h"
#include "bitband.h"


//������ʼ��
void button_init(void)
{
	GPIO_InitTypeDef Button_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	//ͨ��APB2����ʹGPIOCʱ��ʹ��
	
	Button_Value.GPIO_Mode = GPIO_Mode_IPU;//������������
	Button_Value.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;//8��9�ܽ�
	GPIO_Init(GPIOC, &Button_Value); // �ܽų�ʼ��
	
	Button_Value.GPIO_Mode = GPIO_Mode_IPD;//������������
	Button_Value.GPIO_Pin = GPIO_Pin_0;//0�Źܽ�
	GPIO_Init(GPIOA, &Button_Value);
}


//��ⰴ�°���
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











