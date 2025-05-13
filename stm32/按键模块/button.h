/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2025-05-13 21:39:58
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head-1\stm32\按键模块\button.h
 * @Description: 
 */
#ifndef __BUTTON_H__
#define __BUTTON_H__

#define BUTTON_NUM 2

typedef enum
{
    button_pressed,
    button_released,
    button_hold,
    button_none
}button_state_t;

typedef struct button
{
    GPIO_TypeDef *port;
    uint16_t pin;
    button_state_t state;
    uint32_t hold_time;
}button_t;

typedef struct button_fifo
{
    button_state_t buffer[20];
    button_state_t *read;
    button_state_t *write;
}button_fifo_t;

void button_struct_init(button_t *button);

void button_init(void);
void button_scan(void);

#endif
