#include "sht30.h"
#include "gpio_iic.h"
#include "delay.h"

//sht30初始化  == IIC初始化
void sht30_init(void)
{
	IIC_Init();
}

/*
sht30 repeatability mps, measure per second
首先需要将START信号发送，然后发送设备地址写，等待ACK
发送速率命令高字节，等到ACK之后发送命令低字节等待ACK
*/
void sht30_set_measure_speed(void)
{
	IIC_Start();
	IIC_Send_Byte(SHT30_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(REPEAT_MPS>>8);
	IIC_Wait_Ack();
	IIC_Send_Byte(REPEAT_MPS & 0xFF);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*
SHT30 读取温度命令
首先发送START信号，然后发送带写的设备地址，等待ACK，
然后发送高地址0xE0，收到ACK之后发送低地址0x00,然后收到
ACK信号之后再发送START，带读的设备地址，等待ACK。收到之后
便可接收数据，首先是温度高八位数据，发送ACK。然后是温度低
八位小数数据,发送AcK. 然后收到CRC校验数据,发送ACK。
然后是湿度数据，高八位，发ACK，低八位发ACK，CRC发NACK，发送
STOP信号。
*/
void sht30_readout_cmd(u8 buf[6])
{
	int i = 0;
	
	IIC_Start();
	IIC_Send_Byte(SHT30_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(READ_RESULT >> 8);
	IIC_Wait_Ack();
	IIC_Send_Byte(READ_RESULT & 0xFF);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(SHT30_ADDR | 1);
	IIC_Wait_Ack();
	for (i = 0; i < 6; i ++)
	{
		buf[i] = IIC_Recv_Byte(i == 5?0:1);
	}
	IIC_Stop();
}

/*
SHT30 数据转换，将接收到的数据转换成十进制温湿度
*/
void sht30_read_result(double sht_data[2])
{
	u8 buf[6] = {0};
	uint16_t temp, hum;
	
	sht30_readout_cmd(buf);
	temp = buf[0];
	temp = temp << 8 | buf[1];
	hum = buf[3];
	hum = hum << 8 | buf[4];
	
	sht_data[0] = -45.0 + (175.0*((double)temp)/65535.0);
	sht_data[1] = 100.0 * ((double)hum)/65535.0;
}






