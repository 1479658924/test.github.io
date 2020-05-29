/***********************************************************************************
*		基于STM32+GPRS的无线远程智能终端设计
*		外围设备 ：继电器
*								GPRS A6模块
*               OLED显示屏
*								SHT30温湿度传感器
*
*		设备上电后进行SIM卡初始化，然后链接TCP，链接成功之后进行数据传输，上传数据可
*在手机APP上进行查看。设备采用看门狗进行状态监视，一旦程序跑死出现宕机，一段时间
*后程序会重启运行。
*************************************************************************************/

#include "stdio.h"
#include "string.h"
#include "led.h"
#include "buzzer.h"
#include "relay.h"
#include "delay.h"
#include "iwdg.h"
#include "sht30.h"
#include "oled.h"
#include "v_resistor.h"
#include "iot.h"
#include "sim.h"

#define API_PASS "eyJleHAiOjE5MDMzOTkzNDQsImFsZyI6IkhTMjU2IiwiaWF0IjoxNTg4MDM5MzQ0fQ.eyJpZCI6MTQzMX0.ReznaYcaxcaGT5CuUdBtv37ps2ZxAJt6-rr3EHiHqd8:"

#define c_dev_id	"4603"
#define DEV_NUM 	3

int main(void)
{
	int dev_id[DEV_NUM] = {4627, 4628, 4629};
	double dev_data[DEV_NUM] = {0};
	int v_res_value = 0;
	double sht_data[2] = {0};
	char Sht_Temp[9] = {0};
	char Sht_Hum[11] = {0};
	char ADC_Str[11] = {0};
	int ret = 0, err_times = 0;
	static int i = 0;
	
	led_init();
	sht30_init();
	delay_init();
	OLED_Init();
	relay_init();
	OLED_Clear();
	v_res_init();
	iwdg_init(156*24);
	
	fill_picture(0x00);
	sht30_set_measure_speed();
	delay_ms(10);
	OLED_OpenBMP();
	led_on_all();
	sim_init();
	led_off_all();
	led_on(0);
	sim_ready();
	led_off(0);
	iwdg_feed();
	while(1)
	{
		v_res_value = v_res_read_value();
		sht30_read_result(sht_data);
		delay_ms(100);
		dev_data[0] = sht_data[0];
		dev_data[1] = sht_data[1];
		dev_data[2] = v_res_value;
		OLED_Clear();
		sprintf(Sht_Temp, " : %.2f", sht_data[0]);
		sprintf(Sht_Hum, " : %.2f %%", sht_data[1]);
		sprintf(ADC_Str, "ADC  : %04d", v_res_value);
		OLED_ShowTEmp(0, 0);
		OLED_ShowString(30, 0, (u8 *)Sht_Temp, 16);
		OLED_ShowTIttle(100, 0);
		OLED_ShowHUm(0, 4);
		OLED_ShowString(30, 4, (u8 *)Sht_Hum, 16);
		OLED_ShowString(0, 2, (u8*)ADC_Str, 16);
		delay_ms(1000);		

		iwdg_feed();
		err_times = 0;
		OLED_ShowLinking(0, 6);
		do {
			ret = sim_connect("iot.embsky.com", "80");
			if (ret == 1) {
				ret = sim_judge_status();
				if (ret == 1) {
					sim_disconnect();
				} else if (ret == 3) {
					iwdg_feed();
					sim_shut();
					sim_ready();
				}
			}
			err_times ++; 
			OLED_ShowChar((err_times+3)*16, 6, (u8)'.', 16);
			if (err_times == 4) {
				iwdg_feed();
				sim_shut();
				led_on_all();
				sim_init();
				led_off_all();
				led_on(0);
				sim_ready();
				led_off(0);
			}  
			if (err_times > 4)
				break;
		} while (ret != 0);
		iwdg_feed();
		if (i == 1) {
			OLED_Clear_status();
			OLED_ShowUploading(0, 6);
			led_on(1);
			upload_device_datas("iot.embsky.com", "1698", API_PASS, dev_id, dev_data, DEV_NUM);
			led_off(1);
		} else {
			OLED_Clear_status();
			OLED_ShowDownloading(0, 6);
			led_on(2);
			download_sensor_status("iot.embsky.com", "1698", c_dev_id, API_PASS);
			led_off(2);
		}
		OLED_Clear_status();
		err_times = 0;
		do {
			ret = sim_disconnect();
			err_times ++;
		} while (ret == 1 && err_times < 3);

		i = (i + 1) % 3;
	}
}







