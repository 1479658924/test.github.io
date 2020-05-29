#include "v_resistor.h"

static void adc_init(void)
{
	GPIO_InitTypeDef Gpio_Value;
	ADC_InitTypeDef Adc_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AIN;//ƒ£ƒ‚ ‰»Î
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;//0∫≈π‹Ω≈
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
	
	Adc_Value.ADC_ContinuousConvMode = DISABLE;
	Adc_Value.ADC_DataAlign = ADC_DataAlign_Right;
	Adc_Value.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	Adc_Value.ADC_Mode = ADC_Mode_Independent;
	Adc_Value.ADC_NbrOfChannel = 10;
	Adc_Value.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &Adc_Value);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
}

static void adc_start(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

static void adc_wait(void)
{
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
}

static int adc_value(void)
{
	return ADC_GetConversionValue(ADC1);
}	

void v_res_init(void)
{
	adc_init();
}

int v_res_read_value(void)
{
	adc_start();
	adc_wait();
	return adc_value();
}











