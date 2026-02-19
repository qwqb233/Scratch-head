#ifndef __MENU_H__
#define __MENU_H__
#include <stdint.h>
typedef struct
{
	uint8_t CurrentNum;	//当前索引序号:页码
	uint8_t Enter;		//确认键
	uint8_t Next;		//下一个
	uint8_t Return;		//返回键
	void (*Current_Operation)(void);//当前操作(函数指针)
}Menu_table_t;

	uint8_t Mode_Choose(void);
	void Menu_Interface(void);
	void Function_Interface1(void);
	void Function_Interface2(void);
	void Function_Interface3(void);
	void Function_Interface4(void);
	void Function_Interface5(void);
	void Function_Interface6(void);
	

#endif

