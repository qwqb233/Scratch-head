#ifndef __PWM_CONTROLLER_H
#define __PWM_CONTROLLER_H


#include "ti_msp_dl_config.h"

#define DUTY_TO_CCR(duty, arr) arr - ((duty * arr) / 100)

typedef struct
{
    int* timer;
    int index;
    int duty;
    int arr;
}pwm_controller_t;

void PWM_init(pwm_controller_t* pwm, int* timer, int index, int arr);
void PWM_setDuty(pwm_controller_t* pwm, int duty);



#endif /*__PWM_CONTROLLER_H*/
