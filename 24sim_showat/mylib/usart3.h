#ifndef __USART3_H
#define __USART3_H

/*
	�ܽ�  ��/��
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

extern void usart3_init(void); //��ʼ��

extern void usart3_send_byte(unsigned char ch); // ��������

extern unsigned char usart3_recv_byte(void); // ��������

extern void usart3_send_string(char *str); // �����ַ���

extern void usart3_send_struct(data_type *dat); // ���ͽṹ��

extern void usart3_handler_replace(usart3_handler h);

extern void usart3_handler_replace_str(usart3_handler_str h);

extern void usart3_handler_replace_struct(usart3_handler_struct h);

extern int usart3_handler_send_at(char *command, char *ack, int time);



#endif





