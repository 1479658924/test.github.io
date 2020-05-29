#ifndef __USART3_H
#define __USART3_H

/*
	管脚  发/收
	PB11		RX
	PB10		TX
*/

#include "stm32f10x_conf.h"

typedef struct data {
	float Temp;
	char Hem;
	int Pm;
}data_type;

typedef void (*usart3_handler)(void);
typedef void (*usart3_handler_str)(unsigned char *str);
typedef void (*usart3_handler_struct)(data_type *data_struct);

extern void usart3_init(void); //初始化

extern void usart3_send_byte(unsigned char ch); // 发送数据

extern unsigned char usart3_recv_byte(void); // 接收数据

extern void usart3_send_string(char *str); // 发送字符串

extern void usart3_send_struct(data_type *dat); // 发送结构体

extern void usart3_handler_replace(usart3_handler h);

extern void usart3_handler_replace_str(usart3_handler_str h);

extern void usart3_handler_replace_struct(usart3_handler_struct h);

extern int usart3_handler_send_at(char *command, char *ack, int time);



#endif





