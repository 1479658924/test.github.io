#ifndef __USART1_H
#define __USART1_H

/*
	管脚  发/收
	PA10	RX
	PA9		TX
*/

#include "stm32f10x_conf.h"

typedef struct data1 {
	float Ds18b20_Value;
	char Hum_Value;
	int Pm_Value;
}data_t;

typedef void (*usart1_handler)(void);
typedef void (*usart1_handler_str)(unsigned char *str);
typedef void (*usart1_handler_struct)(data_t *da);

extern void usart1_init(void); //初始化

extern void usart1_send_byte(unsigned char ch); // 发送数据

extern unsigned char usart1_recv_byte(void); // 接收数据

extern void usart1_send_string(unsigned char *str); // 发送字符串

extern void usart1_handler_replace(usart1_handler h);

extern void usart1_send_struct(void *struct_data); // 发送结构体

extern void usart1_handler_replace_str(usart1_handler_str h);


#endif





