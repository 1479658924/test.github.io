#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x_conf.h"

typedef void (*timer_handler_type) (void);

extern void timerx_init(void);
//��ʼ��timerx

extern void set_timerx_handler(timer_handler_type h);
//���ûص�����


#endif





