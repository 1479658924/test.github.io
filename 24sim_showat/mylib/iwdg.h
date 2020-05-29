#ifndef __IWDG_H
#define __IWDG_H

#include "stm32f10x_conf.h"

extern void iwdg_init(int ms);//看门狗初始化

extern void iwdg_feed(void);//喂狗


#endif




