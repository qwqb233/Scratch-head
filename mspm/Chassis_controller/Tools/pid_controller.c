#include "pid_controller.h"

pid_controller_t PID_struct_init(pid_controller_t pid_controller)
{
    // default PID parameters, can be changed by user
    pid_controller.P = 0.5;
    pid_controller.I = 0;
    pid_controller.D = 0;
    pid_controller.U = 0;
    pid_controller.err = 0;
    pid_controller.last_err = 0;
    pid_controller.target = 0;
    return pid_controller;
}

pid_controller_t PID_output_calculate(pid_controller_t input)
{
    // calculate the error
    input.err = input.target - input.last_err;

    // calculate the output
    input.U = input.P * input.err + input.I * (input.err + input.last_err) + input.D * (input.err - input.last_err);

    // update the last error
    input.last_err = input.err;

    return input;
}
