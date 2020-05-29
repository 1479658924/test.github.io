#include "esp8266.h"
#include "usart2.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define O_F	1//O��״̬
#define K_F	2//K��״̬
static int flag = O_F;
static int ok_flag = 0;
static int len = 0;//�������ݳ��ȵ�����

//"+IPD,13:1234567890ABC"
#define ADD_S 	0//+��״̬
#define I_S			1//I��״̬
#define P_S			2//P��״̬
#define D_S			3//D��״̬
#define H_S			4//,��״̬
#define LEN_S		5//���ȵ�״̬
#define DATA_S	6//���ݵ�״̬
static int data_flag = ADD_S;//Ĭ����+��״̬
wifi_recv_handler wifi_handler = NULL;

void set_wifi_recv_handler(wifi_recv_handler h)//����wifi���յ����ݵĻص�����
{
	wifi_handler = h;
}

void dataHandler(unsigned char c)//�������յ�������
{
	static char len_buf[5];//���յ����ݵ��ֽ���
	static int i = 0;//ѭ������
	static char data_buf[512];//�洢���յ�������
	
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

void esp8266_init(void)//��ʼ���ĺ���(��ʼ������)
{
	usart2_init();
}

void esp8266_link_wifi(char *ssid, char *passwd)//����·��(��Ҫ·�ɵ����ƺ�·�ɵ�����)
{
	//AT+RST\r\n ��������ģ��
	usart2_send_string("AT+RST\r\n");
	delay_ms(2000);//����֮����ʱ����
	
	//AT+CWMODE=1\r\n ѡ��wifiģʽ
	ok_flag = 0;
	usart2_send_string("AT+CWMODE=1\r\n");//ѡ��ģʽ
	while(ok_flag == 0);//�ȴ�esp8266��OK
	
	//AT+CWJAP="SSID","password"\r\n ����·��
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
	
	//AT+CIFSR\r\n ��ѯ���䵽��IP��ַ(����Ҫ��һ��,�����޷���������)
	usart2_send_string("AT+CIFSR\r\n");
	delay_ms(2000);
}

void esp8266_connect_tcp(char *ip, char *port)//����TCP server
{
	//AT+CIPSTART="TCP","192.168.3.116",8080\r\n ����TCP����
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

void esp8266_disconnect(void)//�Ͽ�TCP����
{
	//AT+CIPCLOSE\r\n �Ͽ�TCP����
	usart2_send_string("AT+CIPCLOSE\r\n");
	//ע��:�п��ܻظ��п��ܲ��ظ�,���Բ����closed ok
	delay_ms(200);
}

void esp8266_send_data(char *msg, char *len)//��������
{
	//AT+CIPSEND=4\r\n ��������
	usart2_send_string("AT+CIPSEND=");
	usart2_send_string(len);
	usart2_send_string("\r\n");
	ok_flag = 0;
	while(ok_flag == 0);//�ȴ� OK
	
	ok_flag = 0;
	while(ok_flag == 0);//�ȴ� >
	
	usart2_send_string(msg);
	ok_flag = 0;
	while(ok_flag == 0);//�ȴ� SEND OK
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

void esp8266_send_data_udp(char *msg, char *len, char *id)//��������
{
	//AT+CIPSEND=4, byte_num\r\n ��������
	ok_flag = 0;
	usart2_send_string("AT+CIPSEND=");
	usart2_send_string(id);
	usart2_send_byte(',');
	usart2_send_string(len);
	usart2_send_string("\r\n");
	while(ok_flag == 0);//�ȴ� OK
	
	ok_flag = 0;
	while(ok_flag == 0);//�ȴ� >
	
	ok_flag = 0;
	usart2_send_string(msg);
	while(ok_flag == 0);//�ȴ� SEND OK
}







