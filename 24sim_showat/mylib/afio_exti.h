#ifndef __AFIO_EXTI_H
#define __AFIO_EXTI_H
/*
	°´¼ü	I/O
	KEY0	PC9
	KEY1	PC8
	KEY2	PA0
*/

typedef void (*handler)(void);

#include "stm32f10x_conf.h"

extern void exti_init(void);

extern void exti_handler(handler h1, handler h2, handler h3);



#endif


