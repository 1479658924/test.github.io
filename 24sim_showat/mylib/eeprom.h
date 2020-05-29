#ifndef __EEPROM_H
#define __EEPROM_H

/*
device address
		24c02		1010 A2 A1 A0 R/W
		其中A2-A0 链接至板子的GND 
		所以device address为0xA0或0xA1
		内部为16(page)*16(byte)
*/

#include "stm32f10x_conf.h"

extern void eeprom_init(void);
//初始化EEPROM

extern void eeprom_write_byte(u8 dev_addr, u8 word_addr, u8 data);
//向EEPROM指定位置存数据

extern void eeprom_write_page(u8 dev_addr, u8 word_addr, u8 *data, u8 len);
//向EEPROM中写指定长度的数据

extern u8 eeprom_read_random(u8 dev_addr, u8 word_addr);
//从EEPROM指定位置读数据

extern u8 eeprom_read_current_address(u8 dev_addr);
//读取上次操作地址的下一地址

extern void eeprom_read_sequ_on_current(u8 dev_addr, u8 *data, u8 len);
//按页读数据

extern void eeprom_read_sequ_on_random(u8 dev_addr, u8 word_addr, u8 *data, u8 len);
//按页读数据

#endif






