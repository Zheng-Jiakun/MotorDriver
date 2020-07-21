#include "user_main.h"
#include "motor_driver.h"
#include "algorithm.h"

pid_t motor_pid;

float motor_current = 0.1f, motor_speed = 0, motor_position;

void user_setup()
{
    motor.pwm = -3;
    motor_start();
}

void user_loop()
{
    // motor_current_loop(motor_current);
    // motor_speed_loop(motor_speed);
    // motor_position_loop(motor_position);

    // motor_speed = pid_calc(&motor_pid_position, motor.position, motor_position);
    // motor_current = pid_calc(&motor_pid_speed, motor.current, motor_speed);
    // motor.pwm = motor_current;
    HAL_Delay(5);
}