#ifndef __RELAY_H
#define __RELAY_H

/*
继电器 扩展IO   PC 4
高电平打开
*/


#include "stm32f10x_conf.h"
	
extern void relay_init(void);

extern void relay_on(void);

extern void relay_off(void);

#endif


