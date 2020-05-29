#include "usart3.h"
#include "string.h"
#include "delay.h"
#include "oled.h"
#include "relay.h"
#include "sim.h"
#include "usart1.h"

#define MAX_U3_LEN	1024
unsigned char u3_str[MAX_U3_LEN] = {0};

int t2_time = 0;
static int Flag = 0;
data_type u3_struct = {0.0, 0, 0};

usart3_handler u_h;
usart3_handler_str Usart3_h;
usart3_handler_struct Usart3_h_struct;

unsigned int sign = 0;
unsigned int u3_locate = 0;

char *at_ack = NULL;

void usart3_init(void) //初始化
{
	GPIO_InitTypeDef Gpio_Value;
	USART_InitTypeDef Usart_Value;
	NVIC_InitTypeDef Nvic_Value;
//	TIM_TimeBaseInitTypeDef TIM2_Value;
	//定时器结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//使能PB口 串口
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;//设置推挽输出
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//设置浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &Gpio_Value);
	
	Usart_Value.USART_BaudRate = 115200;
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Value.USART_Parity = USART_Parity_No;
	Usart_Value.USART_StopBits = USART_StopBits_1;
	Usart_Value.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &Usart_Value);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Nvic_Value.NVIC_IRQChannel = USART3_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART3, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	TIM2_Value.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM2_Value.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM2_Value.TIM_Period = 12000;
//	TIM2_Value.TIM_Prescaler = 5;//12M
//	TIM2_Value.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM2, &TIM2_Value);
//		
//	Nvic_Value.NVIC_IRQChannel = TIM2_IRQn;
//	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
//	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
//	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&Nvic_Value);
//	
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM2, DISABLE);
}

void usart3_send_byte(unsigned char ch) // 发送数据
{
	USART_SendData(USART3, ch);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART3, USART_FLAG_TC);
}

void usart3_send_string(char *str)
{
	int i = 0;
	
	for (i = 0; *(str+i); i ++)
		usart3_send_byte(*(str+i));
}

void usart3_send_struct(data_type *dat)
{
	int i = 0;
	data_type *p = dat;
	
	for (i = 0; i < sizeof(data_type); i ++) {
		usart3_send_byte(*(unsigned char *)p + i);
	}
}

unsigned char usart3_recv_byte(void) // 接收数据
{
	unsigned char ret = 0;
	
	if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET) {
		ret = USART_ReceiveData(USART3);
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
	return ret;
}

void usart3_handler_replace(usart3_handler h)
{
	u_h = h;
}

void usart3_handler_replace_str(usart3_handler_str h)
{
	Usart3_h = h;
}

void usart3_handler_replace_struct(usart3_handler_struct h)
{
	Usart3_h_struct = h;
}

void USART3_IRQHandler(void)
{
	unsigned char buf;
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		//u_h();
//		sim_handler(USART_ReceiveData(USART3));
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		buf = USART_ReceiveData(USART3);     //接收串口3数据到buff缓冲区
		u3_str[u3_locate] = buf;
		u3_locate ++; 
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		sign ++;	
	}
}

int usart3_handler_send_at(char *command, char *ack, int time)
{
	int j = 0;
	char *recv = "sensor_id";
	char *judge = NULL;
	
	usart3_send_string(command);
	delay_ms(time);
	if (strstr((const char *)u3_str, ack) != NULL)
		Flag = 1;
	else 
		Flag = 0;
	
	 if (u3_locate > 256) {
		delay_ms(200);
		judge = strstr((const char *)u3_str, recv);
		if (judge != NULL) {
			if (strstr((const char *)u3_str, "true")) {
				relay_on();
			} else {
				relay_off();
			}
		}
	} 
//	 else if (u3_locate < 256) {
//		OLED_Clear();
//		OLED_ShowString(0, 0, (u8*)u3_str, 12);
//	}
	
	sign = 0;
	u3_locate = 0;
	for (j = 0; j < MAX_U3_LEN; j ++) 
		u3_str[j] = '\0';
	
	return !Flag;
}














