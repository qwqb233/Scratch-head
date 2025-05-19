#include "button.h"
#include <cstddef>

button_t button_list[BUTTON_NUM];

void button_struct_init(button_t *button)
{
    button->pin = 0;
    button->port = NULL;
    button->hold_time = 0;
    button->state = button_none;
}

void button_init(void)
{
    // 创建按键,BUTTON_PIN,BUTTON_PORT为HAL库定义的GPIO宏定义，hold_time为0为关闭长按检测
    // button_t button = {.pin = BUTTON_PIN, .port = BUTTON_PORT, .hold_time = BUTTON_HOLD_TIME, .button_state = button_none};

    // 将button_t结构体设为初值
    // button_t button;
    // button_struct_init(&button);
    // button.pin = BUTTON_PIN;
    // button.port = BUTTON_PORT;
}

void put_key_fifo()

void button_scan(void)
{

}
