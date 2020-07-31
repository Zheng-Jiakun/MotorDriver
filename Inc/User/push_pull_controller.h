#ifndef PUSH_PULL_CONTROLLER
#define PUSH_PULL_CONTROLLER

#include "main.h"
#include "motor_driver.h"

#define CALIBRATING_SPEED   15
#define NORMAL_SPEED        50

#define POSITION_DIFFERENCE 3600U
#define POSITION_TOLERANCE  180

#define PUSH_DISTANCE_RATIO 0.6f

/*
12v
pwm 10 => 0.6 ~ 0.9
pwm 20 => 0.8 ~ 1.5
pwm 30 => 1.0 ~ 2.4
*/
#if (CALIBRATING_SPEED == 15)
    #define CURRENT_UPPER_LIMIT   1.7f
    #define CURRENT_LOWER_LIMIT   0.6f  
#endif

void push_pull_init();
void position_calibrate();
void testing_thread();
void normal_testing();

#endif