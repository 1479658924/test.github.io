#include "sim.h"
#include "delay.h"
#include "usart1.h"
#include "usart3.h"
#include "led.h"
#include "stdio.h"
#include "stdlib.h"
#include "buzzer.h"
#include "relay.h"
#include "string.h"

void sim_init(void)
{
	usart3_init();
	delay_ms(2000);

	usart3_handler_send_at("AT\r\n", "OK", 50);
	usart3_handler_send_at("AT+RST\r\n", "OK", 100);
	usart3_handler_send_at("AT\r\n", "OK", 100);
	usart3_handler_send_at("ATE1\r\n", "OK", 100);
	//AT+COPS? 是否链接运营商
	usart3_handler_send_at("AT+COPS?\r\n", "OK", 1000);	
	
	//AT+CSQ  查询信号
	usart3_handler_send_at("AT+CSQ\r\n", "OK", 1000);	
	
	//AT+CCID  是否插卡
	usart3_handler_send_at("AT+CCID\r\n", "OK", 300);
}

uint32_t sim_ready(void)
{
	int ret = 0;
	
	//AT+CREG? 是否注册上网络
	do {
		ret = usart3_handler_send_at("AT+CREG?\r\n", "OK", 100);
	} while (ret == 1);
		//AT+CGCLASS=B 工作类型
	do {
		ret = usart3_handler_send_at("AT+CGCLASS=B\r\n", "OK", 200);
	} while (ret == 1);
	
//	// 查询波特率
//	usart3_send_string("AT+IPR?\r\n");

	//AT+CGATT=1 附着网络
	do {
		ret = usart3_handler_send_at("AT+CGATT=1\r\n", "OK", 200);
	} while (ret == 1);
	//AT+CGDCONT=1,"IP","CMNET" 指定PDP上下文
	do {
		ret = usart3_handler_send_at("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n", "OK", 400);
	} while (ret == 1);
	//AT+CGACT=1,1 激活PDP上下文
	do {
		ret = usart3_handler_send_at("AT+CGACT=1,1\r\n", "OK", 800);
	} while (ret == 1);
	
	return 0;
}

uint32_t sim_connect(char *ip, char *port)
{
	char send_buf[50];
 
	sprintf(send_buf, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", ip, port);
	return usart3_handler_send_at(send_buf, "CONNECT OK", 2000);
}

uint32_t sim_judge_status(void)
{
	if (usart3_handler_send_at("AT+CIPSTATUS\r\n", "CONNECTING", 300) == 0) {
		return 1;
	} else if (usart3_handler_send_at("AT+CIPSTATUS\r\n", "CLOSE", 300) == 0) {
		return 2;
	} else if (usart3_handler_send_at("AT+CIPSTATUS\r\n", "CONNECT OK", 500) == 0) {
		return 0;
	}
	
	return 3;
}

uint32_t sim_disconnect(void)
{
//	usart3_send_string("+++");
//	delay_ms(500);
	return usart3_handler_send_at("AT+CIPCLOSE\r\n", "OK", 200);
}

uint32_t sim_send(char *msg)
{
	usart3_handler_send_at("AT+CIPSEND\r\n", ">", 500);
	usart3_handler_send_at(msg, "OK", 2000);
	usart3_send_byte(0x1A);
	
	return 0;
}

uint32_t sim_get(char *msg, char *len)
{
	char send[20];

	sprintf(send, "AT+CIPSEND=%s\r\n", len);
	usart3_handler_send_at(send, ">", 500);

	usart3_handler_send_at(msg, "OK", 2000);
	sim_disconnect();
	
	return 0;
}

uint32_t sim_shut(void)
{
	return usart3_handler_send_at("AT+CIPSHUT\r\n", "OK", 200);
}

void sim_reopen_network(void)
{
	int ret = 0;
	
	//AT+CGATT=0 关闭网络
  usart3_handler_send_at("AT+CGATT=0\r\n", "OK", 200);
	
	//AT+CREG? 是否注册上网络
  usart3_handler_send_at("AT+CREG?\r\n", "OK", 200);
	
	//AT+CGATT=1 附着网络
	do {
		ret = usart3_handler_send_at("AT+CGATT=1\r\n", "OK", 200);
	} while (ret == 1);
}

//  AT+CIFSR  获取IP
void sim_get_ip(void)
{
	usart3_handler_send_at("AT+CIFSR\r\n", "OK", 300);
}





























