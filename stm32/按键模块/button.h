/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2025-05-13 21:39:58
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head-1\stm32\按键模块\button.h
 * @Description: 
 */
#ifndef __BUTTON_H__
#define __BUTTON_H__

#define GPIO_PORT_TYPE GPIO_TypeDef*
#define GPIO_PIN_TYPE uint16_t
#define BUTTON_FIFO_SIZE 10

typedef enum
{
    button_none = 0,

    start_button_pressed,
    start_button_long_pressed,
    start_button_released,

    select_button_pressed,
    select_button_long_pressed,
    select_button_released,

    button_max,
} button_t;

typedef struct button_struct
{
    GPIO_PORT_TYPE port;
    GPIO_PIN_TYPE pin;

    uint8_t active_level;

    uint8_t state;
    uint8_t last_state;

    uint32_t hold_time;
    uint32_t count_hold_time;
    uint32_t filter_time;
    uint32_t count_filter_time;
    uint32_t repeat_time;
    uint32_t count_repeat_time;
}button_struct_t;

typedef struct button_fifo
{
    uint8_t fifo[BUTTON_FIFO_SIZE];
    uint8_t * read;
    uint8_t * write;
}button_fifo_t;

#define BUTTON_COUNT (uint8_t)((button_max-1)/3)


extern button_struct_t button_list[BUTTON_COUNT];
extern button_fifo_t button_fifo;

void button_init(void);
void button_scan(void);
uint8_t button_fifo_read(void);
//放置在滴答定时器或一毫秒定时器中
void button_scan_1ms(void);
void button_scan_10ms(void);


#endif
