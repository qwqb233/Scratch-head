#include "pwm_controller.h"
#include "ti_msp_dl_config.h"

void PWM_init(pwm_controller_t* pwm, int* timer, int index, int arr)
{
    pwm->timer = timer;
    pwm->index = index;
    pwm->arr = arr;
    pwm->duty = 0;
}

void PWM_setDuty(pwm_controller_t* pwm, int duty)
{
    if(duty < 0) duty = 0;
    if(duty > 100) duty = 100;
    pwm->duty = duty;
    int ccr_value = DUTY_TO_CCR(duty, pwm->arr);
    DL_TimerG_setCaptureCompareValue(pwm->timer, ccr_value, pwm->index);
}