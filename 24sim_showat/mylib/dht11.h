#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x_conf.h"

/*
#define DHT11_GPIO_TYPE  GPIOC
#define DHT11_GPIO_PIN   GPIO_Pin_10
#define DHT11_RCC        RCC_APB2Periph_GPIOC


#define DHT11_OUT_H GPIO_SetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_OUT_L GPIO_ResetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_IN    GPIO_ReadInputDataBit(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)


void dht11_gpio_input(void);
void dht11_gpio_output(void);
unsigned char dht11_scan(void);
unsigned char dht11_read_bit(void);
unsigned char dht11_read_byte(void);
unsigned char dht11_read_data(unsigned char buffer[5]);
*/
extern void dht11_input_init(void);

extern void dht11_output_init(void);

extern void dht11_set(void);

extern int dht11_read_dat(unsigned char data[5]);

extern int dht11_read_result(double dht_data[2]);

#endif



