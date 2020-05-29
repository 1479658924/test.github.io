#ifndef __SHT30_H
#define __SHT30_H
/*
SHT30 IIC device address 
	addressA 0x44 VSS 
	addressB 0x45 VDD 
	device address read 0x44<<1   0x88
	device address write 0x45<<1  0x8A

SHT30 repeate measure enable MSB 0x2C LSB 0x06/0x0D/0x10
											disable MSB 0x24 LSB 0x00/0x0B/0x16
											

*/

#include "stm32f10x_conf.h"

#define SHT30_ADDR		0x88
#define REPEAT_MPS	0x2737 //ÿ���������
#define READ_RESULT	0xE000 //��ȡ����

extern void sht30_init(void);
//sht30��ʼ��  == IIC��ʼ��

extern void sht30_set_measure_speed(void);
//����sht30��������

extern void sht30_readout_cmd(u8 buf[6]);
//sht30��ȡ�������

extern void sht30_read_result(double sht_data[2]);
//sht30��ȡ��ת�����

#endif








