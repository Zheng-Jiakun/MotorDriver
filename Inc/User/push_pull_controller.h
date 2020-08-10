#ifndef PUSH_PULL_CONTROLLER
#define PUSH_PULL_CONTROLLER

#include "main.h"
#include "motor_driver.h"

extern int16_t motor_position;

#define CALIBRATING_SPEED   15
#define RUNNING_SPEED       40
#define ADJUST_SPEED        10
#define MANUAL_SPEED        15

#define NORMAL_RPM          1000

#define POSITION_DIFFERENCE 3600U
#define POSITION_TOLERANCE  250

#define MIDDLE_POSITION     0.5f

/*
12v
pwm 10 => 0.6 ~ 0.9
pwm 20 => 0.8 ~ 1.5
pwm 30 => 1.0 ~ 2.4
*/
#if (CALIBRATING_SPEED == 15)
    #define CURRENT_UPPER_LIMIT   1.4f
    #define CURRENT_LOWER_LIMIT   0.6f  
#endif

typedef enum {
    AUTO = 0,
    MANUAL,
    ADJUST
} working_mode_t;

typedef enum {
    AUTO_STOP = 0,
    AUTO_RUN,
    MANUAL_PUSHING = 10,
    MANUAL_MIDDLE,
    MANUAL_PULLING,
    ADJUST_PUSHING = 20,
    ADJUST_PULLING,
    ADJUST_STOP
} working_command_t;

typedef enum {
    SPEED_HOLD = 0,
    SPEED_UP,
    SPEED_DOWN
} speed_control_t;

typedef enum {
    STOP = 0,
    CALIBRATING,
    RUNNING,
    PUSHING,
    MIDDLE,
    PULLING
} feedback_state_t;

extern working_mode_t working_mode;
extern feedback_state_t feedback_state;
extern working_command_t working_command;
extern speed_control_t speed_control;

extern int16_t push_limit_position, pull_limit_position;

void push_pull_init();
void position_calibrate();
void testing_thread();
void normal_testing();

#endif