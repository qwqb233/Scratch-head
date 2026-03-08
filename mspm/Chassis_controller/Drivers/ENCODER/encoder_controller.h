#ifndef __ENCODER_CONTROLLER_H__
#define __ENCODER_CONTROLLER_H__

typedef struct
{
    int* timer;
    int index;
    int16_t now_count;
    int16_t err_count;
    int16_t last_count;
} encoder_controller_t;

void encoder_init(encoder_controller_t* encoder, int* timer, int index);
int16_t encoder_get_count(encoder_controller_t* encoder);

#endif /* __ENCODER_CONTROLLER_H__ */
