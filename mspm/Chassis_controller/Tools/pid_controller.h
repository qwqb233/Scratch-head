#ifndef __PID_CONTROLLER_H
#define __PID_CONTROLLER_H

typedef struct
{
    float P;
    float I;
    float D;
    float U;
    float err;
    float last_err;
    float target;
}pid_controller_t;

void PID_struct_init(pid_controller_t* pid_controller);
void PID_output_calculate(pid_controller_t* input);

#endif /*__PID_CONTROLLER_H*/
