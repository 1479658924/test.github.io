#include "gpio_iic.h"
#include "delay.h"
#include "bitband.h"

/*
用GPIO的方式模拟IIC总线这种方法,和硬件连接的GPIO有关
我的设备接到不同的GPIO上,将来就需要配置响应的GPIO
因为我们做的实验有OLED EEPROM SHT30,而这些设备都是连接在了相同的管脚上
所以,在使用时从设备地址一定不要写错
*/

//以下代码是硬件相关的，移植时候需要修改
GPIO_InitTypeDef  GPIO_InitForSCL;
GPIO_InitTypeDef  GPIO_InitForSDAOut;
GPIO_InitTypeDef  GPIO_InitForSDAIn;

#define IIC_SCL					PBOut(6)
#define IIC_SDA					PBOut(7)
#define IIC_SDA_STATUS 	PBIn(7)

void IIC_Init(void)
{			
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//PB6 SCL
  GPIO_InitForSCL.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitForSCL.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitForSCL.GPIO_Speed = GPIO_Speed_50MHz;
	
	//PB7 SDA OUT
	GPIO_InitForSDAOut.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitForSDAOut.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitForSDAOut.GPIO_Speed = GPIO_Speed_50MHz;
	
  //PB7 SDA IN
  GPIO_InitForSDAIn.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitForSDAIn.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitForSDAIn.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOB, &GPIO_InitForSDAOut);
  GPIO_Init(GPIOB, &GPIO_InitForSCL);
	
	IIC_SCL = 1;
	IIC_SDA = 1;
}

void IIC_Sda_In(void)
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAIn);
}

void IIC_Sda_Out(void)
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAOut);
}
//以上代码是硬件相关的，移植时候需要修改

//以下代码硬件无关，只跟iic协议有关
/*
IIC START信号 需要在SCL高电平 SDA由高电平拉到低电平触发
*/
void IIC_Start(void)
{
	IIC_Sda_Out();//设置SDA推挽输出
	IIC_SDA=1;//拉高SDA引脚电平为START做准备	  	  
	IIC_SCL=1;  //将SCL拉高电平
	delay_us(2);
 	IIC_SDA=0; //将SDA电平拉低
	delay_us(2);
	IIC_SCL=0; //拉低SCL可进行通信
}	  

/*
IIC STOP信号 需要在SCL高电平 SDA由低电平拉高触发
*/
void IIC_Stop(void)
{
	IIC_Sda_Out();//设置SDA引脚推挽输出
	IIC_SCL=0;//拉低SCL电平 防止出现错误信号
	IIC_SDA=0;//拉低SDA引脚电平为STOP做准备
 	delay_us(2);
	IIC_SCL=1; //将SCL拉高等待SDA信号
	delay_us(1);
	IIC_SDA=1;//拉高SDA电平 发出STOP信号
	delay_us(2);
}

/*
IIC 等待应答  ACK为低电平 
*/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;//未接受应答变量
	IIC_Sda_In();//设置SDA上拉输入 IPU
	IIC_SCL = 0;//拉低时钟线
	delay_us(1);  
	IIC_SCL=1; //拉高时钟线
	delay_us(1);
	while(IIC_SDA_STATUS) { //未接收到ACK
		ucErrTime++; //计数加
		if(ucErrTime>250) { //超过一定范围
			IIC_Stop();// 停止IIC
			return 1;
		}
	}
	IIC_SCL=0; 	//拉低时钟SCL   
	return 0;  
} 

/*
IIC 发送ACK 在SCL高电平状态时 ACK低电平
*/
void IIC_Ack(void)
{
	IIC_SCL=0; //拉低SCL
	IIC_Sda_Out();//设置SDA推挽
	IIC_SDA=0;//拉低SDA
	delay_us(2);
	IIC_SCL=1;//拉高SCL 等待ACK
	delay_us(2);
	IIC_SCL=0;//拉低SCL
}

/*
IIC 不发送ACK SCL高电平状态时 NACK为高电平
*/
void IIC_NAck(void)
{
	IIC_SCL=0;//拉低SCL
	IIC_Sda_Out();//设置SDA推挽
	IIC_SDA=1;//拉高SDA
	delay_us(2);
	IIC_SCL=1;//拉高SCl 接收NACK
	delay_us(2);
	IIC_SCL=0;//拉低SCL 防止出现错误信号接受
}					 				     

/*
IIC 1byte数据发送
*/
void IIC_Send_Byte(u8 txd)
{                        
  u8 t; // unsigned char变量 
	
	IIC_Sda_Out(); //设置SDA推挽   

  for (t = 0;t < 8; t++) { //循环8次 将整个的参数放入到SDA 
		IIC_SCL = 0;//拉低SCL 可进行数据接收
		delay_us(1);	
    IIC_SDA = (txd & 0x80) >> 7;//将参数传入的数据高位放入SDA
    txd <<= 1; //将参数左移一位 一遍进行下次数据	  
		delay_us(1);   
		IIC_SCL=1; //拉高SCL 1bit数据完成
		delay_us(2); 
   }	
	IIC_SCL = 0; //拉低SCL 可进行下次SDA数据接受
} 	    

/*
IIC 接收1byte数据
*/
u8 IIC_Recv_Byte(unsigned char ack)
{
	unsigned char i,receive=0;//循环变量 接收数据变量
	IIC_Sda_In();//设置SDA上拉
  for(i=0;i<8;i++ ) {
    IIC_SCL=0; //拉低SCL
    delay_us(2); 
		IIC_SCL=1; //拉高SCL
		delay_us(2); 
    receive<<=1; //接收数据变量移位 进行数据接受
    if(IIC_SDA_STATUS) //如果SDA信号为高电平
			receive |= 1; //接受数据变量赋值  
  }					 
	IIC_SCL = 0;//拉低SCL
  if (!ack)//判断参数 是否需要发送ACK
		IIC_NAck(); //不发送
  else
		IIC_Ack(); //发送
  return receive;
}
