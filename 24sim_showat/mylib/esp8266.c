#include "esp8266.h"
#include "usart2.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define O_F	1//O的状态
#define K_F	2//K的状态
static int flag = O_F;
static int ok_flag = 0;
static int len = 0;//保存数据长度的整数

//"+IPD,13:1234567890ABC"
#define ADD_S 	0//+的状态
#define I_S			1//I的状态
#define P_S			2//P的状态
#define D_S			3//D的状态
#define H_S			4//,的状态
#define LEN_S		5//长度的状态
#define DATA_S	6//数据的状态
static int data_flag = ADD_S;//默认是+的状态
wifi_recv_handler wifi_handler = NULL;

void set_wifi_recv_handler(wifi_recv_handler h)//设置wifi接收到数据的回调函数
{
	wifi_handler = h;
}

void dataHandler(unsigned char c)//处理接收到的数据
{
	static char len_buf[5];//接收到数据的字节数
	static int i = 0;//循环变量
	static char data_buf[512];//存储接收到的数据
	
	switch(data_flag)
	{
		case ADD_S :
									if(c == '+')
										data_flag = I_S;
									break;
		case I_S :
									if(c == 'I')
										data_flag = P_S;
									else
										data_flag = ADD_S;
									break;
		case P_S :
									if(c == 'P')
										data_flag = D_S;
									else
										data_flag = ADD_S;
									break;
		case D_S :
									if(c == 'D')
										data_flag = H_S;
									else
										data_flag = ADD_S;
									break;
		case H_S :
									if(c == ',')
										data_flag = LEN_S;
									else
										data_flag = ADD_S;
									break;
		case LEN_S ://+IPD,13:1234567890ABC
									if(c == ':')
									{
										len_buf[i] = '\0';
										len = atoi(len_buf);
										data_flag = DATA_S;
										i = 0;
										break;
									}
									else
										len_buf[i] = c;
									i++;
									break;
		case DATA_S:
									data_buf[i] = c;
									i++;
									if(i == len)
									{
										i = 0;										
										if(wifi_handler)
											wifi_handler(data_buf, len);
										data_flag = ADD_S;
										memset(data_buf, '\0', 512);
										break;
									}
									break;
	}
	
	if(c == '>')
		ok_flag = 1;
	
	if(ok_flag == 0)
	{
		switch(flag)
		{
			case O_F : 
									if(c == 'O')
										flag = K_F;
									break;
			case K_F : 
									if(c == 'K')
									{
										flag = O_F;
										ok_flag = 1;
									}
									else
										flag = O_F;
									break;
		}
	}
}

void esp8266_init(void)//初始化的函数(初始化串口)
{
	usart2_init();
}

void esp8266_link_wifi(char *ssid, char *passwd)//连接路由(需要路由的名称和路由的密码)
{
	//AT+RST\r\n 重启无线模块
	usart2_send_string("AT+RST\r\n");
	delay_ms(2000);//重启之后延时两秒
	
	//AT+CWMODE=1\r\n 选择wifi模式
	ok_flag = 0;
	usart2_send_string("AT+CWMODE=1\r\n");//选择模式
	while(ok_flag == 0);//等待esp8266的OK
	
	//AT+CWJAP="SSID","password"\r\n 连接路由
	ok_flag = 0;
	usart2_send_string("AT+CWJAP=");
	usart2_send_byte('"');
	usart2_send_string(ssid);
	usart2_send_byte('"');
	usart2_send_byte(',');
	usart2_send_byte('"');
	usart2_send_string(passwd);
	usart2_send_byte('"');
	usart2_send_string("\r\n");
	while(ok_flag == 0);
	
	//AT+CIFSR\r\n 查询分配到的IP地址(必须要查一下,否则无法建立连接)
	usart2_send_string("AT+CIFSR\r\n");
	delay_ms(2000);
}

void esp8266_connect_tcp(char *ip, char *port)//连接TCP server
{
	//AT+CIPSTART="TCP","192.168.3.116",8080\r\n 建立TCP连接
	usart2_send_string("AT+CIPSTART=");
	usart2_send_byte('"');
	usart2_send_string("TCP");
	usart2_send_byte('"');
	usart2_send_byte(',');
	usart2_send_byte('"');
	usart2_send_string(ip);
	usart2_send_byte('"');
	usart2_send_byte(',');
	usart2_send_string(port);
	usart2_send_string("\r\n");
	ok_flag = 0;
	while(ok_flag == 0);
}

void esp8266_disconnect(void)//断开TCP连接
{
	//AT+CIPCLOSE\r\n 断开TCP连接
	usart2_send_string("AT+CIPCLOSE\r\n");
	//注意:有可能回复有可能不回复,所以不检测closed ok
	delay_ms(200);
}

void esp8266_send_data(char *msg, char *len)//发送数据
{
	//AT+CIPSEND=4\r\n 发送数据
	usart2_send_string("AT+CIPSEND=");
	usart2_send_string(len);
	usart2_send_string("\r\n");
	ok_flag = 0;
	while(ok_flag == 0);//等待 OK
	
	ok_flag = 0;
	while(ok_flag == 0);//等待 >
	
	usart2_send_string(msg);
	ok_flag = 0;
	while(ok_flag == 0);//等待 SEND OK
}

void esp8266_connect_udp(char *ip, char *id, char *port_send, char *port_recv)
{
	usart2_send_string("AT+CIPMUX=1\r\n");
	ok_flag = 0;
	while (ok_flag == 0);
	ok_flag = 0;
	usart2_send_string("AT+CIPSTART=");
	usart2_send_string(id);
	usart2_send_byte('"');
	usart2_send_string("UDP");
	usart2_send_byte('"');
	usart2_send_byte(',');
	usart2_send_byte('"');
	usart2_send_string(ip);
	usart2_send_byte('"');
	usart2_send_byte(',');
	usart2_send_string(port_send);
	usart2_send_byte(',');
	usart2_send_string(port_recv);
	usart2_send_string(",0");
	usart2_send_string("\r\n");
	while(ok_flag == 0);
}

void esp8266_send_data_udp(char *msg, char *len, char *id)//发送数据
{
	//AT+CIPSEND=4, byte_num\r\n 发送数据
	ok_flag = 0;
	usart2_send_string("AT+CIPSEND=");
	usart2_send_string(id);
	usart2_send_byte(',');
	usart2_send_string(len);
	usart2_send_string("\r\n");
	while(ok_flag == 0);//等待 OK
	
	ok_flag = 0;
	while(ok_flag == 0);//等待 >
	
	ok_flag = 0;
	usart2_send_string(msg);
	while(ok_flag == 0);//等待 SEND OK
}








