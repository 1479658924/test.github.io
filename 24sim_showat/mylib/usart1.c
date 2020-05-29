#include "usart1.h"
#include "led.h"

#define Max_BUFF_Len 18
unsigned char Uart1_Buffer[Max_BUFF_Len];
unsigned int Uart1_Rx = 0;
unsigned int u1_sign = 0;

usart1_handler u1_h;
usart1_handler_str Usart1_h;
usart1_handler_struct Usart1_struct_h;
unsigned char u1_str[Max_BUFF_Len];

void usart1_init(void) //初始化
{
	GPIO_InitTypeDef Gpio_Value;
	USART_InitTypeDef Usart_Value;
	NVIC_InitTypeDef Nvic_Value;
	TIM_TimeBaseInitTypeDef TIM4_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	//使能PA口 串口
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;//设置推挽输出
	Gpio_Value.GPIO_Pin = GPIO_Pin_9;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//设置浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Usart_Value.USART_BaudRate = 115200;
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Value.USART_Parity = USART_Parity_No;
	Usart_Value.USART_StopBits = USART_StopBits_1;
	Usart_Value.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &Usart_Value);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Nvic_Value.NVIC_IRQChannel = USART1_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM4_Value.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM4_Value.TIM_CounterMode = TIM_CounterMode_Up;
	TIM4_Value.TIM_Period = 12000;
	TIM4_Value.TIM_Prescaler = 5;//12M
	TIM4_Value.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM4_Value);
		
	Nvic_Value.NVIC_IRQChannel = TIM4_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&Nvic_Value);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, DISABLE);
}

void usart1_send_byte(unsigned char ch) // 发送数据
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void usart1_send_string(unsigned char *str)
{
	int i = 0;
	
	for (i = 0; *(str+i); i ++)
		usart1_send_byte(*(str+i));
}

unsigned char usart1_recv_byte(void) // 接收数据
{
	unsigned char ret = 0;
	
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {
		ret = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	return ret;
}


void usart1_handler_replace(usart1_handler h)
{
	u1_h = h;
}

void usart1_handler_replace_str(usart1_handler_str h)
{
	Usart1_h = h;
}

void usart1_handler_replace_struct(usart1_handler_struct h)
{
	Usart1_struct_h = h;
}
/*
void USART1_IRQHandler(void)
{
	int i = 0;
	unu1_signed char buf;
	data_type *p;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
	//	u1_h();
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		buf = USART_ReceiveData(USART1);	
		*((unu1_signed char *)p+i++) = buf;
		if (i == sizeof(data_type)) {
			Usart1_struct_h(p);
			i = 0;
		}
	}
}
*/

void USART1_IRQHandler(void) 
{
	unsigned char buf;
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //中断产生 
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志
		buf = USART_ReceiveData(USART1);     //接收串口1数据到buff缓冲区
		Uart1_Buffer[Uart1_Rx] = buf;
		Uart1_Rx++; 
	}
}

void TIM4_IRQHandler(void)
{
	int j = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		u1_sign ++;
		if (u1_sign == 2) {
			TIM_Cmd(TIM4, DISABLE);
			Usart1_h(Uart1_Buffer);
			u1_sign = 0;
			Uart1_Rx = 0;
			for (j = 0; j < Max_BUFF_Len; j ++) 
				Uart1_Buffer[j] = '\0';
		}
	}
}



