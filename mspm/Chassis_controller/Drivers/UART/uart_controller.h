/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2026-02-19 17:08:07
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head\mspm\Chassis_controller\Drivers\UART\uart_controller.h
 * @Description: 
 */
#ifndef __UART_CONTROLLER_H
#define __UART_CONTROLLER_H

#include "ti_msp_dl_config.h"

#define RING_BUFFER_SIZE 512

void UART_init(void);
void UART_send_byte(uint8_t byte);
void UART_send_bytes(uint8_t* bytes, uint32_t length);

#endif /*__UART_CONTROLLER_H*/
