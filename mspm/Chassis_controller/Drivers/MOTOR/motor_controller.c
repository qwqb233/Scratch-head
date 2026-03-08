/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2026-02-20 10:51:36
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \Scratch-head\mspm\Chassis_controller\Drivers\MOTOR\motor_controller.c
 * @Description: 
 */
#include "motor_controller.h"

void motor_init(MotorStatus *motor, int* timer, int index, int arr, float kp, float ki, float kd)
{
    PWM_init(&motor->pwm, timer, index, arr);
    PID_struct_init(&motor->pid);
    motor->pid.P = kp;
    motor->pid.I = ki;
    motor->pid.D = kd;
    motor->pid.target = 0;
}

void set_target_speed(MotorStatus *motor, int16_t target_speed)
{
    motor->target_speed = target_speed;
    motor->pid.target = target_speed;
}

void motor_get_speed(MotorStatus *motor)
{
    encoder_get_count(&motor->encoder);
    // TODO: 计算速度
    motor->now_speed = motor->encoder.err_count;
}


void motor_control_pid(MotorStatus *motor)
{
    PID_output_calculate(&motor->pid);
    // TODO: 计算占空比
    int duty = (int)motor->pid.U;
    if (duty > 100)
    {
        duty = 100;
    }
    else if (duty < -100)
    {
        duty = -100;
    }
    PWM_setDuty(&motor->pwm, duty);
}
