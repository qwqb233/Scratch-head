#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//PB12
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;					
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
}

//低电平触发
void Buzzer_beep(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);		//将PB12引脚设置为低电平，蜂鸣器鸣叫
	Delay_ms(300);							//延时100ms
	GPIO_SetBits(GPIOB, GPIO_Pin_12);		//将PB12引脚设置为高电平，蜂鸣器停止
	Delay_ms(300);	
}