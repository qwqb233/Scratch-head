/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2025-05-15 21:26:12
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head-1\stm32\按键模块\button_test.c
 * @Description: 
 */
//
// Created by qwqb233 on 2025/5/19.
//
#include "main.h"
#include "myMain.h"


int my_while()
{
    button_init();
    uint32_t time_counter = 0;
    while(1)
    {
        button_scan_1ms();

        time_counter++;
        uint8_t buffer[1024];
        if(time_counter >= 500)
        {
            uint8_t read = button_fifo_read();
            switch(read)
            {
                case start_button_pressed:
                    HAL_UART_Transmit(&huart1,"start\r\n",strlen("start\r\n"),1000);
                    break;
                case start_button_released:
                    HAL_UART_Transmit(&huart1,"start re\r\n",strlen("start re\r\n"),1000);
                    break;
                case start_button_long_pressed:
                    HAL_UART_Transmit(&huart1,"start lp\r\n",strlen("start lp\r\n"),1000);
                    break;
                case select_button_pressed:
                    HAL_UART_Transmit(&huart1,"select\r\n",strlen("select\r\n"),1000);
                    break;
                case select_button_released:
                    HAL_UART_Transmit(&huart1,"select re\r\n",strlen("select re\r\n"),1000);
                    break;
                case select_button_long_pressed:
                    HAL_UART_Transmit(&huart1,"select lp\r\n",strlen("select lp\r\n"),1000);
                    break;
                default:
                    sprintf(buffer,"nothing %d \r\n",read);
                    //HAL_UART_Transmit(&huart1,buffer,strlen(buffer),1000);
                    break;
            }
            HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
            time_counter = 0;
        }
        HAL_Delay(1);
    }
}
