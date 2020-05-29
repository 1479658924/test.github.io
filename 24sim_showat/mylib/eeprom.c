#include "eeprom.h"
#include "gpio_iic.h"

//��ʼ��EEPROM
void eeprom_init(void)
{
	IIC_Init();
}

//��EEPROMָ��λ�ô�����
/*
д�ֽ�����SDAҪ����START��Ȼ����device address ���һλ
��ʾ����д���ú�������Ϊд�����Ե�ַΪ0xA0����λ�ȴ���
Ȼ��ȴ�ACKӦ�𣬷���Ӧ��֮����word address��ָ���洢����
λ�ã�Ȼ����data�ȴ�ACK������STOP�ź����д
*/
void eeprom_write_byte(u8 dev_addr, u8 word_addr, u8 data)
{
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(word_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*
��ҳд��ͬ����SDA�Ͻ��У����ȷ���START�źţ�Ȼ����device 
address ��0xA0�����յ�ACK�źź���word address,�Ƚ��յ�ACK
֮�����data(0)-data(len-1)�ķ��͡�
��ҳд���ڻع�(��ַ�ӵ�16����)��������֮ǰ����
�������Ҫ����ACK����STOP
*/
void eeprom_write_page(u8 dev_addr, u8 word_addr, \
	u8 *data, u8 len)
{
	int i = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(word_addr);
	IIC_Wait_Ack();
	for (i = 0; i < len; i ++) {
		IIC_Send_Byte(data[i]);
		IIC_Wait_Ack();
	}
	IIC_Stop();
}

/*
random read:��ȡָ��λ�õ�����
������Ҫ����dummy write(��д,����START��device address�ȴ�ACK,
����word address,�ȴ�ACK),����START������device address(��ʱ���λΪ1)
Ȼ��ȴ�ACK���������ݶ�ȡ��֮��ֱ��STOP����
*/
u8 eeprom_read_random(u8 dev_addr, u8 word_addr)
{
	u8 data = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(word_addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(dev_addr | 1);
	IIC_Wait_Ack();
	data = IIC_Recv_Byte(0);
	IIC_Stop();
	
	return data;
}

/*
current address read��
first START send,then send device address,and wait ACK,
then read data, finally send STOP
*/
u8 eeprom_read_current_address(u8 dev_addr)
{
	u8 data = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev_addr|1);
	IIC_Wait_Ack();
	data = IIC_Recv_Byte(0);
	IIC_Stop();
	
	return data;
}

/*
sequential read:
��ҳ��ȡ����
���Ȱ���current address read�ķ�ʽ��ȡ��Ȼ�����֮ǰҪ����ACK
���Ž����´����ݵĶ����Ӷ�ʵ���˰�ҳ����
*/
void eeprom_read_sequ_on_current(u8 dev_addr, \
	u8 *data, u8 len)
{
	int i = 0;
	IIC_Start();
	IIC_Send_Byte(dev_addr|1);
	IIC_Wait_Ack();
	for (i = 0; i < len; i ++) {
		data[i] = IIC_Recv_Byte(i==len-1?0:1);
	}
	IIC_Stop();
}

/*
sequential read:
��ҳ��ȡ����
���Ȱ���random read�ķ�ʽ��ȡ��Ȼ�����֮ǰҪ����ACK
���Ž����´����ݵĶ����Ӷ�ʵ���˰�ҳ����
*/
void eeprom_read_sequ_on_random(u8 dev_addr, \
	u8 word_addr, u8 *data, u8 len)
{
	int i = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(word_addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(dev_addr | 1);
	IIC_Wait_Ack();
	for (i = 0; i < len; i ++) {
		data[i] = IIC_Recv_Byte(i==len-1?0:1);
	}
	IIC_Stop();
}



