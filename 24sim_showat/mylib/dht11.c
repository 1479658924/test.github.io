#include "dht11.h"
#include "bitband.h"
#include "delay.h"

void dht11_input_init(void)
{
	GPIO_InitTypeDef Gpio_Value;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
}

void dht11_output_init(void)
{
	GPIO_InitTypeDef Gpio_Value;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
}

void dht11_set(void)
{
	dht11_output_init();// 输出
	PCOut(10) = 0;
	delay_ms(20);
	PCOut(10) = 1;
	delay_us(30); // 建立通信
}

static unsigned char dht11_read_byte(void)
{
	int i = 0;
	unsigned char data = 0;
	
	for (i = 0; i < 8; i ++) {
		data <<= 1;
		while (PCIn(10) == 0);
		delay_us(35);
		if (PCIn(10)) {
			data |= PCIn(10);
			while (PCIn(10));
		}
	}
	
	return data;
}

int dht11_read_dat(unsigned char data[5])
{
	unsigned int i = 0;
	dht11_set();
	dht11_input_init();//接受电平
	if (PCIn(10) == 0) {
		while (PCIn(10) == 0);
		while (PCIn(10) == 1);
		
		for (i = 0; i < 5; i ++) {
			data[i] = dht11_read_byte();
		}
		while (PCIn(10) != 0);
		dht11_output_init();// 输出
		PCIn(10) = 1;
		if (data[4] == data[0] + data[1] + data[2] + data[3])
			return 0;
	}
	return 1;
}

int dht11_read_result(double dht_data[2])
{
	unsigned char data[5] = {0};
	
	if (!dht11_read_dat(data)) {
		dht_data[1] = data[0] + data[1] / 10.0;//tempreture
		dht_data[0] = data[2] + data[3] / 10.0;//hum
		return 0;
	}
	return 1;
}

/*
void dht11_gpio_input(void)
{
    GPIO_InitTypeDef g;
    
    RCC_APB2PeriphClockCmd(DHT11_RCC, ENABLE);
    
    g.GPIO_Pin = DHT11_GPIO_PIN;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    
    GPIO_Init(DHT11_GPIO_TYPE, &g);
}

void dht11_gpio_output(void)
{
    GPIO_InitTypeDef g;
    
    RCC_APB2PeriphClockCmd(DHT11_RCC, ENABLE);
    
    g.GPIO_Pin = DHT11_GPIO_PIN;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出

    GPIO_Init(DHT11_GPIO_TYPE, &g);
}

void dht11_reset(void)
{
    // 按照DHT11手册步骤
    dht11_gpio_output();
    DHT11_OUT_L;
    delay_us(19000);
    DHT11_OUT_H;
    delay_us(30);
    dht11_gpio_input();
}

unsigned char dht11_scan(void)
{
    return DHT11_IN;
}

unsigned char dht11_read_bit(void)
{
    while (DHT11_IN == RESET);
    delay_us(40);
    if (DHT11_IN == SET)
    {
        while (DHT11_IN == SET);
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char dht11_read_byte(void)
{
    int i;
    unsigned char data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

unsigned char dht11_read_data(unsigned char buffer[5])
{
    int i = 0;
    unsigned char checksum;
    dht11_reset();
    if (dht11_scan() == RESET)
    {
        //检测到DHT11响应
        while (dht11_scan() == RESET);
        while (dht11_scan() == SET);
        for (i = 0; i < 5; i++)
        {
            buffer[i] = dht11_read_byte();
        }
        
        while (dht11_scan() == RESET);
        dht11_gpio_output();
        DHT11_OUT_H;
        
        checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
        if (checksum != buffer[4])
        {
            // checksum error
            return 1;
        }
    }
    
    return 0;
}

*/






