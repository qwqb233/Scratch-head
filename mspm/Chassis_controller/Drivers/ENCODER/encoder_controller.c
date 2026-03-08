#include "encoder_controller.h"

void encoder_init(encoder_controller_t* encoder, int* timer, int index)
{
    encoder->timer = timer;
    encoder->index = index;
    encoder->now_count = 0;
    encoder->last_count = 0;
    encoder->err_count = 0;
}

void encoder_get_count(encoder_controller_t *encoder)
{
    encoder->last_count = encoder->now_count;
    encoder->now_count = DL_TIMER_getCaptureValue(encoder->timer, encoder->index);
    encoder->err_count = encoder->now_count - encoder->last_count;
    if (encoder->err_count > 32767)
    {
        encoder->err_count -= 65536;
    }
    else if (encoder->err_count < -32768)
    {
        encoder->err_count += 65536;
    }
}
