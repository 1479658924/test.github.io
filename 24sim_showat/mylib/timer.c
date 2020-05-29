#include "timer.h"
#include "stdio.h"

timer_handler_type t_h = NULL;

void timerx_init(void)
{
	NVIC_InitTypeDef Nvic_Value;
	TIM_TimeBaseInitTypeDef TIMx_Value;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIMx_Value.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMx_Value.TIM_CounterMode = TIM_CounterMode_Up;
	TIMx_Value.TIM_Period = 12000;
	TIMx_Value.TIM_Prescaler = 5;//12M
	TIMx_Value.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIMx_Value);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	Nvic_Value.NVIC_IRQChannel = TIM3_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, DISABLE);
}

void set_timerx_handler(timer_handler_type h)
{
	t_h = h;
}

void TIM3_IRQHandler(void)
{
	static int i = 0;
	TIM_SetCounter(TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		i ++;
		if (i == 1000) {
			t_h();
			i = 0;
		}
	}
}














