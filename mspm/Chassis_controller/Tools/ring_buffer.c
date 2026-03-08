/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2026-02-19 16:22:33
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head\mspm\Chassis_controller\Tools\ring_buffer.c
 * @Description: 
 */
#include "ring_buffer.h"
#include "ti_msp_dl_config.h"


// use a global variable to store the ring buffer state
// don't use in multi modules, otherwise it will cause conflicts
ring_buffer_t ring_buffer;

/*
* @brief  Initializes the ring buffer.
* @param  buffer: Pointer to the buffer array.
* @param  size: Size of the buffer array.
* @retval RING_BUFFER_OK if initialization is successful, otherwise RING_BUFFER_ERROR.
*/
ring_buffer_status_t ring_buffer_init(uint8_t *buffer, uint16_t size)
{
    ring_buffer.buffer = buffer;
    ring_buffer.size = size;
    ring_buffer.head = buffer;
    ring_buffer.tail = buffer;
    return RING_BUFFER_OK;
} 

/*
* @brief  Pushes data into the ring buffer.
* @param  data: Data to be pushed into the buffer.
* @retval RING_BUFFER_FULL if the buffer is full, otherwise RING_BUFFER_OK.
*/
ring_buffer_status_t ring_buffer_push(uint8_t data)
{
    if (ring_buffer_is_full() == RING_BUFFER_FULL) {
        return RING_BUFFER_FULL;
    }
    *(ring_buffer.head) = data;
    ring_buffer.head++;
    if (ring_buffer.head >= ring_buffer.buffer + ring_buffer.size) {
        ring_buffer.head = ring_buffer.buffer;
    }
    return RING_BUFFER_OK;
}

/*
* @brief  Checks if the ring buffer is empty.
* @param  data: Pointer to the data to be read from the buffer.
* @retval RING_BUFFER_EMPTY if the buffer is empty, otherwise RING_BUFFER_OK.
*/
ring_buffer_status_t ring_buffer_pop(uint8_t *data)
{
    if (ring_buffer_is_empty() == RING_BUFFER_EMPTY) {
        return RING_BUFFER_EMPTY;
    }
    *data = *(ring_buffer.tail);
    ring_buffer.tail++;
    // return to buffer head
    if (ring_buffer.tail >= ring_buffer.buffer + ring_buffer.size) {
        ring_buffer.tail = ring_buffer.buffer;
    }
    return RING_BUFFER_OK;
}

/*
* @brief  Checks if the ring buffer is full.
* @retval RING_BUFFER_FULL if the buffer is full, otherwise RING_BUFFER_OK.
*/
ring_buffer_status_t ring_buffer_is_full()
{
    if ((ring_buffer.head + 1 == ring_buffer.tail) ||
        (ring_buffer.head == ring_buffer.buffer + ring_buffer.size - 1 && ring_buffer.tail == ring_buffer.buffer)) {
        return RING_BUFFER_FULL;
    }
    return RING_BUFFER_OK;
}

/*
* @brief  Checks if the ring buffer is empty.
* @retval RING_BUFFER_EMPTY if the buffer is empty, otherwise RING_BUFFER_OK.
*/
ring_buffer_status_t ring_buffer_is_empty()
{
    if (ring_buffer.head == ring_buffer.tail) {
        return RING_BUFFER_EMPTY;
    }
    return RING_BUFFER_OK;
}