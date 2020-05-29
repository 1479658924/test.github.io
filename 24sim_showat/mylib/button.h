#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x_conf.h"

//按键初始化
extern void button_init(void);
//检测按下按键
extern int button_switch(int button_num);


#endif

