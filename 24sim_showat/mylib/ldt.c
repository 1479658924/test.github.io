#include "ldt.h"
#include "bitband.h"
#include "delay.h"

#define HC138_A0		PCOut(11)
#define HC138_A1		PCOut(12)
#define HC138_A2		PDOut(2)
#define HC595_SLCK	PBOut(5)
#define HC595_LCLK	PBOut(4)
#define HC595_DATA	PBOut(3)

void ldt_init(void)//初始化的函数(初始化模块使用到的管脚)
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
	RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//通过APB2总线使能GPIOC组 GPIOB组 GPIOD组 和AFIO的时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//改变指定管脚的映射
	//JTAG-DP 关闭功能 + SW-DP 使能
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//选择 11 12 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择50MHz的输出速度
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;//选择 2 号管脚
	GPIO_Init(GPIOD, &Gpio_Value);//按照上述配置初始化GPIOD组的管脚
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//选择3 4 5号管脚
	GPIO_Init(GPIOB, &Gpio_Value);//按照上述配置初始化GPIOB组的管脚
	
	HC138_A0 = 0;
	HC138_A1 = 0;
	HC138_A2 = 0;
	HC595_SLCK = 0;
	HC595_LCLK = 0;
	HC595_DATA = 0;
}

void hc138_out_data(uint8_t data)//通过hc138选择数码管的公共端
{//按照74HC138的官方手册上的真值表配置函数
	switch (data) {
		case 0: HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 0; break;
		case 1: HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 1; break;
		case 2: HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 0; break;
		case 3: HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 1; break;
		default: break;
	}
}

void hc595_send_data(uint8_t data)//通过hc595把串行数据转换成并行数据
{//按照74HC595的官方手册上的流程配置函数
	int i = 0;
	
	for (i = 0; i < 8; i ++) {
		/*HC595_SLCK = 0;
		HC595_DATA = (data << i) & 0x80;
		HC595_SLCK = 1;*/
		HC595_SLCK = 0;//把HC595_SCLK拉低
		if(data & 0x80)//判断data数据中的最高位是否为真
			HC595_DATA = 1;//把HC595_DATA拉高
		else
			HC595_DATA = 0;//把HC595_DATA拉低
		data <<= 1;//data按位左移等于1
		HC595_SLCK = 1;//把HC595_SCLK拉高
	}
	HC595_LCLK = 0;
	HC595_LCLK = 1;
}

void digit_show_data(int data[], int loc)//在数码管中显示数据
{//使用的是4位共阴极的数码管,自己去找编码 0x3F 0x06 ... (0 ~ 9 A ~ F)
	int i = 0;
	
	uint8_t table[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//存放数码管的编码
	
	for (i = 0; i < loc; i ++) {
		hc138_out_data(i);//选择公共端(选择让哪个数码管显示数据)
		hc595_send_data(table[data[i]]);//把编码通过函数转换成并行数据
		delay_ms(1);
		hc595_send_data(0);//手动清空74HC595模块中8位位移寄存器中的数据
	}
}

void seg_display(int data)
{
	int i = 0;
	int table[4] = {0};
	
	table[0] = data/1000%10;
	table[1] = data/100%10;
	table[2] = data/10%10;
	table[3] = data%10;

	for (i = 0; i < 200; i ++) {
		digit_show_data(table, 4);
		delay_us(500);
	}
}






