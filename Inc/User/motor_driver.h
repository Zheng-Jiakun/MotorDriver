#ifndef MOTOR_DRIVER
#define MOTOR_DRIVER

#define PWM_DUTYCYCLE               20
#define PWM_DUTYCYCLE2PULSE(x)      ((float)(x)/100.0f*62)

#include "main.h"
#include "tim.h"

typedef enum {
    HIGH_ON_LOW_OFF = 0,
    HIGH_OFF_LOW_ON,
    HIGH_OFF_LOW_OFF
} mosfet_state_t;

void motor_start();
void motor_change_phase();
void get_speed();

#endif