#ifndef __MOTOR_CONTROLLER_H__
#define __MOTOR_CONTROLLER_H__

#include "ti_msp_dl_config.h"
#include "pwm_controller.h"
#include "pid_controller.h"
#include "encoder_controller.h"

typedef struct
{
    pwm_controller_t pwm;
    pid_controller_t pid;
    encoder_controller_t encoder;
    int16_t now_speed;
    int16_t target_speed;
} MotorStatus;

void motor_init(MotorStatus *motor, int* timer, int index, int arr, float kp, float ki, float kd);
void set_target_speed(MotorStatus *motor, int16_t target_speed);
void motor_get_speed(MotorStatus *motor);
void motor_control_pid(MotorStatus *motor);

#endif /* __MOTOR_CONTROLLER_H__ */
