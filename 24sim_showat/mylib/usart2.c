#include "usart2.h"
#include "esp8266.h"

#define MAX_U2_LEN	20
char u2_str[MAX_U2_LEN] = {0};

usart2_handler u2_h;
usart2_handler_str Usart2_h;
usart2_handler_char Usart2_h_char;
usart2_handler_struct Usart2_h_struct;

unsigned int u2_sign = 0;
unsigned int u2_locate = 0;

void usart2_init(void) //初始化
{
	GPIO_InitTypeDef Gpio_Value;
	USART_InitTypeDef Usart_Value;
	NVIC_InitTypeDef Nvic_Value;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//使能PB口 串口
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;//设置推挽输出
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//设置浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Usart_Value.USART_BaudRate = 115200;
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Value.USART_Parity = USART_Parity_No;
	Usart_Value.USART_StopBits = USART_StopBits_1;
	Usart_Value.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &Usart_Value);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Nvic_Value.NVIC_IRQChannel = USART2_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}

void usart2_send_byte(char ch) // 发送数据
{
	USART_SendData(USART2, ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

void usart2_send_string(char *str)
{
	int i = 0;
	
	for (i = 0; *(str+i); i ++)
		usart2_send_byte(*(str+i));
}

unsigned char usart2_recv_byte(void) // 接收数据
{
	unsigned char ret = 0;
	
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) {
		ret = USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	return ret;
}

void usart2_handler_replace(usart2_handler h)
{
	u2_h = h;
}

void usart2_handler_replace_str(usart2_handler_str h)
{
	Usart2_h = h;
}

void usart2_handler_replace_char(usart2_handler_char h)
{
	Usart2_h_char = h;
}

void usart2_handler_replace_struct(usart2_handler_struct h)
{
	Usart2_h_struct = h;
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		dataHandler(USART_ReceiveData(USART2));
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}










