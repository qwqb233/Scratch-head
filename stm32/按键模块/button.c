/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2025-05-13 21:40:04
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head-1\stm32\按键模块\button.c
 * @Description: 
 */
#include "main.h"

#include "button.h"

button_struct_t button_list[BUTTON_COUNT];
button_fifo_t button_fifo;

// #define BUTTON_HARD_INIT //需要硬件初始化


static inline void put_fifo(uint8_t data)
{
    // 计算下一个写位置
    uint8_t *next_write = button_fifo.write + 1;

    // 回绕检查
    if (next_write >= button_fifo.fifo + BUTTON_FIFO_SIZE) 
    {
        next_write = button_fifo.fifo;
    }

    // 检查是否缓冲区满（下一个写位置等于读位置）
    if (next_write == button_fifo.read) 
    {
        return; // 缓冲区满，丢弃数据或处理溢出
    }

    // 写入数据并更新写指针
    *button_fifo.write = data;
    button_fifo.write = next_write;
}


static inline uint8_t isKeyPressed(button_struct_t *button)
{
    if (button->port == NULL)
    {
        return 0;
    }
    if (HAL_GPIO_ReadPin(button->port, button->pin) == button->active_level)
    {
        return 1;
    }
    return 0;
}

static inline void button_struct_init(button_struct_t *button)
{
    button->port = NULL;
    button->pin = 0;
    button->active_level = 0;
    button->state = 0;
    button->last_state = 0;
    button->hold_time = 0;
    button->count_hold_time = 0;
    button->filter_time = 50;
    button->count_filter_time = button->filter_time/2;
    button->repeat_time = 0;
    button->count_repeat_time = 0;
}

static inline void button_hard_init(button_struct_t *button)
{
   //自己实现
}


void button_init(void)
{
    
    #ifdef BUTTON_HARD_INIT
    for(uint8_t i = 0;i < BUTTON_COUNT;i++)
    {
        button_struct_init(&button_list[i]);
        button_hard_init(&button_list[i]);  
    }
    #else
    for(uint8_t i = 0;i < BUTTON_COUNT;i++)
    {
        button_struct_init(&button_list[i]);
    }
    #endif

    button_fifo.read = button_fifo.fifo;
    button_fifo.write = button_fifo.fifo;

    button_list[0].port = Start_button_GPIO_Port;
    button_list[0].pin = Start_button_Pin;
    button_list[0].hold_time = 10;

    button_list[1].port = Select_button_GPIO_Port;
    button_list[1].pin = Select_button_Pin;
    button_list[1].hold_time = 10;
}

void button_scan(void)
{
    for(int i = 0;i < BUTTON_COUNT;i++)
    {
        if(isKeyPressed(&button_list[i]) == 1)
        {
            if(button_list[i].state == 0)
            {
                button_list[i].last_state = button_list[i].state;
                button_list[i].count_hold_time = 0;
                button_list[i].state = 1;
                button_list[i].count_repeat_time = 0;
                button_list[i].count_filter_time = 0;
            }
            else
            {
                button_list[i].count_filter_time++;
                if(button_list[i].count_filter_time >= button_list[i].filter_time)
                {
                    if(button_list[i].count_hold_time == 0) {put_fifo(i*3+1);}//按键按下
                    button_list[i].count_filter_time = 0;
                    button_list[i].count_hold_time++;
                    
                    if(button_list[i].count_hold_time == button_list[i].hold_time && button_list[i].hold_time != 0)
                    {
                        put_fifo(i*3+1+1);//长按
                    }
                    else if(button_list[i].repeat_time > 0 && button_list[i].count_hold_time > button_list[i].hold_time && button_list[i].hold_time != 0)
                    {
                        put_fifo(i*3+1);//按键按下
                        button_list[i].count_repeat_time++;
                    }
                }
                if(button_list[i].count_repeat_time >= button_list[i].repeat_time && button_list[i].repeat_time != 0)
                {
                    put_fifo(i*3+1+1);//重复按下
                }
            }
        }
        else
        {
            button_list[i].count_filter_time++;
            if(button_list[i].state == 1 && button_list[i].count_filter_time >= button_list[i].filter_time)
            {
                button_list[i].last_state = button_list[i].state;
                button_list[i].state = 0;
                button_list[i].count_hold_time = 0;
                button_list[i].count_repeat_time = 0;
                button_list[i].count_filter_time = 0;
                
                put_fifo(i*3+1+2);//按键释放
            }
        }
    }
}

void button_scan_1ms(void)
{
   button_scan();
}

void button_scan_10ms(void)
{
    static uint32_t count = 0;
    count++;
    if(count >= 10)
    {
        button_scan();
        count = 0;
    }
}

uint8_t button_fifo_read(void)
{
    if(button_fifo.read == button_fifo.write)
    {
        return 0xff;
    }
    uint8_t data = *button_fifo.read;
    uint8_t buffer[100];
    button_fifo.read++;
    if(button_fifo.read >= button_fifo.fifo + BUTTON_FIFO_SIZE)
    {
        button_fifo.read = button_fifo.fifo;
    }
    return data;
    //处理数据
}
