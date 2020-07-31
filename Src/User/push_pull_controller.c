#include "push_pull_controller.h"

uint8_t calibrated_flag = 0;
int16_t push_limit_position, pull_limit_position;

void push_pull_init()
{
    motor.pwm = CALIBRATING_SPEED;
}

void position_calibrate()
{
    static uint8_t stuck_flag = 0, hit_count = 0;
    if (motor.current > CURRENT_UPPER_LIMIT && stuck_flag == 0)
    {
        motor.pwm = -motor.pwm;

        if (hit_count % 2)
            pull_limit_position = motor.position;
        else
            push_limit_position = motor.position;

        stuck_flag = 1;
        hit_count++;
    }
    else if (motor.current < CURRENT_LOWER_LIMIT)
    {
        stuck_flag = 0;

        if (push_limit_position - pull_limit_position > POSITION_DIFFERENCE)
            calibrated_flag = 1;
    }
}

void set_motor_pwm(int8_t pwm)
{
    if (motor.pwm > pwm)
        motor.pwm -= (motor.pwm - pwm) * 0.25f + 0.5f;
    else if (motor.pwm < pwm)
        motor.pwm += (pwm - motor.pwm) * 0.25f + 0.5f;
}

void normal_testing()
{
    static uint8_t stuck_flag = 0;
    static int8_t motor_pwm = 0;

    if (motor.position > (push_limit_position - POSITION_TOLERANCE) && stuck_flag == 0)
    {
        motor_pwm = -NORMAL_SPEED;
        stuck_flag = 1;
    }
    else if ((motor.position < pull_limit_position + POSITION_TOLERANCE) && stuck_flag == 0)
    {
        motor_pwm = NORMAL_SPEED;
        stuck_flag = 1;
    }
    else if (motor.position < (push_limit_position - 2 * POSITION_TOLERANCE) && motor.position > (pull_limit_position + 2 * POSITION_TOLERANCE))
    {
        stuck_flag = 0;
    }

    set_motor_pwm(motor_pwm);
    // HAL_Delay(1);
}

void testing_thread()
{
    if (calibrated_flag == 0)
        position_calibrate();
    else
        normal_testing();
}
