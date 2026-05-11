#include "pid.h"
#include <Arduino.h>   // pentru constrain()

void init_PID(PIDController *pid, double kp, double ki, double kd)
{
    pid->kp         = kp;
    pid->ki         = ki;
    pid->kd         = kd;
    pid->prev_error = 0.0;
    pid->integral   = 0.0;
}

double update_PID(PIDController *pid, int setpoint, int measured_value, double dt)
{
    int error = setpoint - measured_value;

    // Termen integral cu anti-windup
    pid->integral += error * dt;
    pid->integral  = constrain(pid->integral, -200.0, 200.0);

    // Termen derivativ
    double derivative = (error - pid->prev_error) / dt;

    double corectie = (pid->kp * error)
                    + (pid->ki * pid->integral)
                    + (pid->kd * derivative);

    pid->prev_error = error;
    return corectie;
}
