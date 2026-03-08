/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2026-02-19 16:22:27
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head\mspm\Chassis_controller\Tools\ring_buffer.h
 * @Description: 
 */
#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include "ti_msp_dl_config.h"

typedef enum
{
    RING_BUFFER_OK = 0,
    RING_BUFFER_ERROR = 1,
    RING_BUFFER_FULL = 2,
    RING_BUFFER_EMPTY = 3
}ring_buffer_status_t;

typedef struct
{
    uint8_t *buffer;
    uint8_t* head;
    uint8_t* tail;
    uint16_t size;
}ring_buffer_t;



ring_buffer_status_t ring_buffer_init(uint8_t *buffer, uint16_t size);
ring_buffer_status_t ring_buffer_push(uint8_t data);
ring_buffer_status_t ring_buffer_pop(uint8_t *data);
ring_buffer_status_t ring_buffer_is_full();
ring_buffer_status_t ring_buffer_is_empty();



#endif /*__RING_BUFFER_H*/
