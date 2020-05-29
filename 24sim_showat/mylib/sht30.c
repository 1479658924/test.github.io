#include "sht30.h"
#include "gpio_iic.h"
#include "delay.h"

//sht30��ʼ��  == IIC��ʼ��
void sht30_init(void)
{
	IIC_Init();
}

/*
sht30 repeatability mps, measure per second
������Ҫ��START�źŷ��ͣ�Ȼ�����豸��ַд���ȴ�ACK
��������������ֽڣ��ȵ�ACK֮����������ֽڵȴ�ACK
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
SHT30 ��ȡ�¶�����
���ȷ���START�źţ�Ȼ���ʹ�д���豸��ַ���ȴ�ACK��
Ȼ���͸ߵ�ַ0xE0���յ�ACK֮���͵͵�ַ0x00,Ȼ���յ�
ACK�ź�֮���ٷ���START���������豸��ַ���ȴ�ACK���յ�֮��
��ɽ������ݣ��������¶ȸ߰�λ���ݣ�����ACK��Ȼ�����¶ȵ�
��λС������,����AcK. Ȼ���յ�CRCУ������,����ACK��
Ȼ����ʪ�����ݣ��߰�λ����ACK���Ͱ�λ��ACK��CRC��NACK������
STOP�źš�
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
SHT30 ����ת���������յ�������ת����ʮ������ʪ��
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






