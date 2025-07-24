#include "menu.h"
#include "button.h"
#include "oled.h"
#include <stdio.h>

//模式选择状态
extern uint8_t state = 0;

uint8_t taskIndex = 0;	//任务调度序号

//菜单页面  //当前，确认，下一个，返回
Menu_table_t taskTable[] =
{
    //一级界面
    {0, 1, 0, 0, Menu_Interface}, 
    //二级界面
    {1, 4, 2, 0, Function_Interface1},
    {2, 5, 3, 0, Function_Interface2},
    {3, 6, 1, 0, Function_Interface3},
	//-- 三级界面
	{4, 4, 4, 1, Function_Interface4},
	{5, 5, 5, 2, Function_Interface5},
	{6, 6, 6, 3, Function_Interface6},
};


uint8_t Mode_Choose(void)
{
	button_scan_1ms(); // 扫描按键状态，更新 FIFO
	uint8_t Key ;
	while ((Key = button_fifo_read()) != 0xff)
	{
		uint8_t button_index = (Key - 1) / 3; // 获取按键索引
		uint8_t event_type = Key - button_index * 3; // 获取事件类型

		switch (button_index)
		{
			// 处理第一个按键
			case 0:
				switch(event_type)
					{
						case 1:
							
							taskIndex = taskTable[taskIndex].Next;
							break;
						case 2:
							
							break;
						case 3:
							
							break;
					}
				break;
			// 处理第二个按键
			case 1:
				switch(event_type)
					{
						case 1:
						
							//button2单击 确认开启
							taskIndex = taskTable[taskIndex].Enter;
							OLED_Clear();
							break;
						case 2:
						
						//长击返回
							taskIndex = taskTable[taskIndex].Return;
							OLED_Clear();
							state = 0;
							break;
						case 3:
							
							break;
					}
				break;

				default:
					OLED_ShowString(6,4,"Unknown button event",12, 0); 
					break;
			}
	
	}
	taskTable[taskIndex].Current_Operation();//执行函数
	return state;
}

/**
  * @brief  菜单界面函数
  * @param  无
  * @retval 无
  */
void Menu_Interface(void)
{
	char buff[50]="WELCOME";
	OLED_ShowString(6,4,buff,16, 0); 
}

 /**
  * @brief  功能界面函数
  * @param  无
  * @retval 无
  */
void Function_Interface1(void)
{
	OLED_ShowString(0, 0, ">>MODE_1",16,1);
	OLED_ShowString(0, 2,  "  MODE_2", 16,0);
	OLED_ShowString(0, 4, "  MODE_3", 16,0);
	
}
void Function_Interface2(void)
{
	OLED_ShowString(0, 0, "  MODE_1", 16,0);
	OLED_ShowString(0, 2, ">>MODE_2", 16,1);
	OLED_ShowString(0, 4, "  MODE_3", 16,0);
}
void Function_Interface3(void)
{
	OLED_ShowString(0, 0, "  MODE_1", 16,0);
	OLED_ShowString(0, 2, "  MODE_2", 16,0);
	OLED_ShowString(0, 4, ">>MODE_3", 16,1);
}
 
/**
  * @brief  功能设置界面函数
  * @param  设置有三种状态
  * @retval 无
  */
void Function_Interface4(void)
{

	OLED_ShowString(4, 1, "  MOVE TO:", 16,0);
	OLED_ShowString(35, 5, "  1  ", 16,1);
	state = 1;
}
void Function_Interface5(void)
{   
	 OLED_ShowCHinese(0,4,0,1); //反相显示汉字“独”
	 OLED_ShowCHinese(16,4,1,1);//反相显示汉字“角”
	 OLED_ShowCHinese(32,4,2,1);//反相显示汉字“兽” 
	state = 2;
}

void Function_Interface6(void)
{
	OLED_DrawBMP(90,0,122, 4,BMP1,0);//正相显示图片BMP1
	state = 3;
}
