#include "ldt.h"
#include "bitband.h"
#include "delay.h"

#define HC138_A0		PCOut(11)
#define HC138_A1		PCOut(12)
#define HC138_A2		PDOut(2)
#define HC595_SLCK	PBOut(5)
#define HC595_LCLK	PBOut(4)
#define HC595_DATA	PBOut(3)

void ldt_init(void)//��ʼ���ĺ���(��ʼ��ģ��ʹ�õ��Ĺܽ�)
{
	GPIO_InitTypeDef Gpio_Value;//������GPIO�̼��Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
	RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//ͨ��APB2����ʹ��GPIOC�� GPIOB�� GPIOD�� ��AFIO��ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ı�ָ���ܽŵ�ӳ��
	//JTAG-DP �رչ��� + SW-DP ʹ��
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ����������ģʽ
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//ѡ�� 11 12 �Źܽ�
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//ѡ��50MHz������ٶ�
	GPIO_Init(GPIOC, &Gpio_Value);//�����������ó�ʼ��GPIOC��Ĺܽ�
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;//ѡ�� 2 �Źܽ�
	GPIO_Init(GPIOD, &Gpio_Value);//�����������ó�ʼ��GPIOD��Ĺܽ�
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//ѡ��3 4 5�Źܽ�
	GPIO_Init(GPIOB, &Gpio_Value);//�����������ó�ʼ��GPIOB��Ĺܽ�
	
	HC138_A0 = 0;
	HC138_A1 = 0;
	HC138_A2 = 0;
	HC595_SLCK = 0;
	HC595_LCLK = 0;
	HC595_DATA = 0;
}

void hc138_out_data(uint8_t data)//ͨ��hc138ѡ������ܵĹ�����
{//����74HC138�Ĺٷ��ֲ��ϵ���ֵ�����ú���
	switch (data) {
		case 0: HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 0; break;
		case 1: HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 1; break;
		case 2: HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 0; break;
		case 3: HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 1; break;
		default: break;
	}
}

void hc595_send_data(uint8_t data)//ͨ��hc595�Ѵ�������ת���ɲ�������
{//����74HC595�Ĺٷ��ֲ��ϵ��������ú���
	int i = 0;
	
	for (i = 0; i < 8; i ++) {
		/*HC595_SLCK = 0;
		HC595_DATA = (data << i) & 0x80;
		HC595_SLCK = 1;*/
		HC595_SLCK = 0;//��HC595_SCLK����
		if(data & 0x80)//�ж�data�����е����λ�Ƿ�Ϊ��
			HC595_DATA = 1;//��HC595_DATA����
		else
			HC595_DATA = 0;//��HC595_DATA����
		data <<= 1;//data��λ���Ƶ���1
		HC595_SLCK = 1;//��HC595_SCLK����
	}
	HC595_LCLK = 0;
	HC595_LCLK = 1;
}

void digit_show_data(int data[], int loc)//�����������ʾ����
{//ʹ�õ���4λ�������������,�Լ�ȥ�ұ��� 0x3F 0x06 ... (0 ~ 9 A ~ F)
	int i = 0;
	
	uint8_t table[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//�������ܵı���
	
	for (i = 0; i < loc; i ++) {
		hc138_out_data(i);//ѡ�񹫹���(ѡ�����ĸ��������ʾ����)
		hc595_send_data(table[data[i]]);//�ѱ���ͨ������ת���ɲ�������
		delay_ms(1);
		hc595_send_data(0);//�ֶ����74HC595ģ����8λλ�ƼĴ����е�����
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






