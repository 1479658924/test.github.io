#include "gpio_iic.h"
#include "delay.h"
#include "bitband.h"

/*
��GPIO�ķ�ʽģ��IIC�������ַ���,��Ӳ�����ӵ�GPIO�й�
�ҵ��豸�ӵ���ͬ��GPIO��,��������Ҫ������Ӧ��GPIO
��Ϊ��������ʵ����OLED EEPROM SHT30,����Щ�豸��������������ͬ�Ĺܽ���
����,��ʹ��ʱ���豸��ַһ����Ҫд��
*/

//���´�����Ӳ����صģ���ֲʱ����Ҫ�޸�
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
//���ϴ�����Ӳ����صģ���ֲʱ����Ҫ�޸�

//���´���Ӳ���޹أ�ֻ��iicЭ���й�
/*
IIC START�ź� ��Ҫ��SCL�ߵ�ƽ SDA�ɸߵ�ƽ�����͵�ƽ����
*/
void IIC_Start(void)
{
	IIC_Sda_Out();//����SDA�������
	IIC_SDA=1;//����SDA���ŵ�ƽΪSTART��׼��	  	  
	IIC_SCL=1;  //��SCL���ߵ�ƽ
	delay_us(2);
 	IIC_SDA=0; //��SDA��ƽ����
	delay_us(2);
	IIC_SCL=0; //����SCL�ɽ���ͨ��
}	  

/*
IIC STOP�ź� ��Ҫ��SCL�ߵ�ƽ SDA�ɵ͵�ƽ���ߴ���
*/
void IIC_Stop(void)
{
	IIC_Sda_Out();//����SDA�����������
	IIC_SCL=0;//����SCL��ƽ ��ֹ���ִ����ź�
	IIC_SDA=0;//����SDA���ŵ�ƽΪSTOP��׼��
 	delay_us(2);
	IIC_SCL=1; //��SCL���ߵȴ�SDA�ź�
	delay_us(1);
	IIC_SDA=1;//����SDA��ƽ ����STOP�ź�
	delay_us(2);
}

/*
IIC �ȴ�Ӧ��  ACKΪ�͵�ƽ 
*/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;//δ����Ӧ�����
	IIC_Sda_In();//����SDA�������� IPU
	IIC_SCL = 0;//����ʱ����
	delay_us(1);  
	IIC_SCL=1; //����ʱ����
	delay_us(1);
	while(IIC_SDA_STATUS) { //δ���յ�ACK
		ucErrTime++; //������
		if(ucErrTime>250) { //����һ����Χ
			IIC_Stop();// ֹͣIIC
			return 1;
		}
	}
	IIC_SCL=0; 	//����ʱ��SCL   
	return 0;  
} 

/*
IIC ����ACK ��SCL�ߵ�ƽ״̬ʱ ACK�͵�ƽ
*/
void IIC_Ack(void)
{
	IIC_SCL=0; //����SCL
	IIC_Sda_Out();//����SDA����
	IIC_SDA=0;//����SDA
	delay_us(2);
	IIC_SCL=1;//����SCL �ȴ�ACK
	delay_us(2);
	IIC_SCL=0;//����SCL
}

/*
IIC ������ACK SCL�ߵ�ƽ״̬ʱ NACKΪ�ߵ�ƽ
*/
void IIC_NAck(void)
{
	IIC_SCL=0;//����SCL
	IIC_Sda_Out();//����SDA����
	IIC_SDA=1;//����SDA
	delay_us(2);
	IIC_SCL=1;//����SCl ����NACK
	delay_us(2);
	IIC_SCL=0;//����SCL ��ֹ���ִ����źŽ���
}					 				     

/*
IIC 1byte���ݷ���
*/
void IIC_Send_Byte(u8 txd)
{                        
  u8 t; // unsigned char���� 
	
	IIC_Sda_Out(); //����SDA����   

  for (t = 0;t < 8; t++) { //ѭ��8�� �������Ĳ������뵽SDA 
		IIC_SCL = 0;//����SCL �ɽ������ݽ���
		delay_us(1);	
    IIC_SDA = (txd & 0x80) >> 7;//��������������ݸ�λ����SDA
    txd <<= 1; //����������һλ һ������´�����	  
		delay_us(1);   
		IIC_SCL=1; //����SCL 1bit�������
		delay_us(2); 
   }	
	IIC_SCL = 0; //����SCL �ɽ����´�SDA���ݽ���
} 	    

/*
IIC ����1byte����
*/
u8 IIC_Recv_Byte(unsigned char ack)
{
	unsigned char i,receive=0;//ѭ������ �������ݱ���
	IIC_Sda_In();//����SDA����
  for(i=0;i<8;i++ ) {
    IIC_SCL=0; //����SCL
    delay_us(2); 
		IIC_SCL=1; //����SCL
		delay_us(2); 
    receive<<=1; //�������ݱ�����λ �������ݽ���
    if(IIC_SDA_STATUS) //���SDA�ź�Ϊ�ߵ�ƽ
			receive |= 1; //�������ݱ�����ֵ  
  }					 
	IIC_SCL = 0;//����SCL
  if (!ack)//�жϲ��� �Ƿ���Ҫ����ACK
		IIC_NAck(); //������
  else
		IIC_Ack(); //����
  return receive;
}
