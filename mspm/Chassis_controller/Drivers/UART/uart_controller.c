#include "uart_controller.h"
#include "ti_msp_dl_config.h"
#include "ring_buffer.h"


uint8_t uart_rx_buffer[RING_BUFFER_SIZE]; // 0.5kB


void UART_init()
{
    // init in main func
    // SYSCFG_DL_init();

    // clear UART interrupt flag and enable UART interrupt
    NVIC_ClearPendingIRQ(UART_DE_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_DE_INST_INT_IRQN);
    ring_buffer_init(uart_rx_buffer, RING_BUFFER_SIZE);
}

void UART_send_byte(uint8_t byte)
{
    while( DL_UART_isBusy(UART_DE_INST) == true );
    DL_UART_Main_transmitData(UART_DE_INST, byte);
}

void UART_send_bytes(uint8_t *bytes, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        UART_send_byte(bytes[i]);
    }
}

//串口的中断服务函数
void UART_DE_INST_IRQHandler(void)
{
    switch( DL_UART_getPendingInterrupt(UART_DE_INST) )
    {
        uint8_t uart_data;
        case DL_UART_IIDX_RX:
            uart_data = DL_UART_Main_receiveData(UART_DE_INST);
            // add data to ring buffer
            ring_buffer_push(uart_data);
            // echo
            UART_send_byte(uart_data);
            break;

        default:
            break;
    }
}

