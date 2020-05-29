#ifndef __USART2_H
#define __USART2_H

/*
	管脚  发/收
	PA3		RX
	PA2		TX
*/

#include "stm32f10x_conf.h"

typedef struct data {
	float Temp;
	char Hem;
	int Pm;
}data_type;

typedef void (*usart2_handler)(void);
typedef void (*usart2_handler_str)(char *str);
typedef void (*usart2_handler_char)(unsigned char ch);
typedef void (*usart2_handler_struct)(data_type *data_struct);

extern void usart2_init(void); //初始化

extern void usart2_send_byte(char ch); // 发送数据

extern unsigned char usart2_recv_byte(void); // 接收数据

extern void usart2_send_string(char *str); // 发送字符串

extern void usart2_handler_replace(usart2_handler h);

extern void usart2_handler_replace_str(usart2_handler_str h);

extern void usart2_handler_replace_char(usart2_handler_char h);

extern void usart2_handler_replace_struct(usart2_handler_struct h);

#endif





