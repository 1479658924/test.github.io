#ifndef __EEPROM_H
#define __EEPROM_H

/*
device address
		24c02		1010 A2 A1 A0 R/W
		����A2-A0 ���������ӵ�GND 
		����device addressΪ0xA0��0xA1
		�ڲ�Ϊ16(page)*16(byte)
*/

#include "stm32f10x_conf.h"

extern void eeprom_init(void);
//��ʼ��EEPROM

extern void eeprom_write_byte(u8 dev_addr, u8 word_addr, u8 data);
//��EEPROMָ��λ�ô�����

extern void eeprom_write_page(u8 dev_addr, u8 word_addr, u8 *data, u8 len);
//��EEPROM��дָ�����ȵ�����

extern u8 eeprom_read_random(u8 dev_addr, u8 word_addr);
//��EEPROMָ��λ�ö�����

extern u8 eeprom_read_current_address(u8 dev_addr);
//��ȡ�ϴβ�����ַ����һ��ַ

extern void eeprom_read_sequ_on_current(u8 dev_addr, u8 *data, u8 len);
//��ҳ������

extern void eeprom_read_sequ_on_random(u8 dev_addr, u8 word_addr, u8 *data, u8 len);
//��ҳ������

#endif






