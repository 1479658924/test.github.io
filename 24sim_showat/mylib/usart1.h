#ifndef __USART1_H
#define __USART1_H

/*
	�ܽ�  ��/��
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

extern void usart1_init(void); //��ʼ��

extern void usart1_send_byte(unsigned char ch); // ��������

extern unsigned char usart1_recv_byte(void); // ��������

extern void usart1_send_string(unsigned char *str); // �����ַ���

extern void usart1_handler_replace(usart1_handler h);

extern void usart1_send_struct(void *struct_data); // ���ͽṹ��

extern void usart1_handler_replace_str(usart1_handler_str h);


#endif





