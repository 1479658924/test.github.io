#include "eeprom.h"
#include "gpio_iic.h"

//初始化EEPROM
void eeprom_init(void)
{
	IIC_Init();
}

//向EEPROM指定位置存数据
/*
写字节首先SDA要发送START，然后发送device address 最后一位
表示读或写，该函数功能为写，所以地址为0xA0，高位先传。
然后等待ACK应答，返回应答之后发送word address，指定存储数据
位置，然后发送data等待ACK，发送STOP信号完成写
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
按页写：同样在SDA上进行，首先发送START信号，然后发送device 
address 即0xA0，当收到ACK信号后发送word address,等接收到ACK
之后继续data(0)-data(len-1)的发送。
按页写存在回滚(地址加到16产生)，即覆盖之前数据
最后依旧要接受ACK后发送STOP
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
random read:读取指定位置的数据
首先需要发送dummy write(假写,发送START、device address等待ACK,
发送word address,等待ACK),发送START，发送device address(此时最低位为1)
然后等待ACK，进行数据读取，之后直接STOP即可
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
current address read：
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
按页读取数据
首先按照current address read的方式读取，然后结束之前要发送ACK
接着进行下次数据的读，从而实现了按页读。
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
按页读取数据
首先按照random read的方式读取，然后结束之前要发送ACK
接着进行下次数据的读，从而实现了按页读。
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



