#include "push_pull_controller.h"

uint8_t calibrated_flag = 0;
int16_t push_limit_position, pull_limit_position;

int8_t running_pwm = RUNNING_SPEED;

working_mode_t working_mode;
feedback_state_t feedback_state;
working_command_t working_command;
speed_control_t speed_control;

int16_t motor_position = 0;

void push_pull_init()
{
    // pid_init(&motor_pid_speed, 0, 50, 50, 0.008f, 0.001f, 0.0008f);
    pid_init(&motor_pid_position, -30, 30, 50, 0.15f, 0.0001f, 0.01f);

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
        {
            calibrated_flag = 1;
            motor_position = pull_limit_position + (push_limit_position - pull_limit_position) * MIDDLE_POSITION;
        }
    }
}

void normal_testing()
{
    static uint8_t stuck_flag = 0;
    // static int16_t motor_rpm = 0;

    if (motor.position > (push_limit_position - POSITION_TOLERANCE) && stuck_flag == 0)
    {
        // motor.pwm = -running_pwm;
        // motor_rpm = -NORMAL_RPM;
        motor_position = pull_limit_position;
        stuck_flag = 1;
    }
    else if ((motor.position < pull_limit_position + POSITION_TOLERANCE) && stuck_flag == 0)
    {
        // motor.pwm = running_pwm;
        // motor_rpm = NORMAL_RPM;
        motor_position = push_limit_position;
        stuck_flag = 1;
    }
    else if (motor.position < (push_limit_position - 2 * POSITION_TOLERANCE) && motor.position > (pull_limit_position + 2 * POSITION_TOLERANCE))
    {
        stuck_flag = 0;
    }

    // if (motor_rpm >= 0)
    //     motor.pwm = pid_calc(&motor_pid_speed, motor.rpm, motor_rpm);
    // else
    //     motor.pwm = -pid_calc(&motor_pid_speed, -motor.rpm, -motor_rpm);

    motor.pwm = pid_calc(&motor_pid_position, motor.position, motor_position);

    HAL_Delay(LOOP_CONTROL_PERIOD);
}

void testing_thread()
{
    if (calibrated_flag == 0)
    {
        position_calibrate();
        feedback_state = CALIBRATING;
    }
    else if (working_mode == AUTO)
    {
        if (working_command == AUTO_RUN)
        {
            feedback_state = RUNNING;
            normal_testing();
        }
        else if (working_command == AUTO_STOP)
        {
            feedback_state = STOP;
            motor.pwm = 0;
        }
        if (speed_control == SPEED_UP)
        {
            if (running_pwm < 60)
                running_pwm++;
        }
        else if (speed_control == SPEED_DOWN)
        {
            if (running_pwm > 3)
                running_pwm--;
        }
        HAL_Delay(20);
    }
    else if (working_mode == MANUAL)
    {
        if (working_command == MANUAL_PUSHING)
        {
            motor_position = push_limit_position - POSITION_TOLERANCE;
            feedback_state = PUSHING;
        }
        else if (working_command == MANUAL_MIDDLE)
        {
            motor_position = pull_limit_position + (push_limit_position - pull_limit_position) * MIDDLE_POSITION;
            feedback_state = MIDDLE;
        }
        else if (working_command == MANUAL_PULLING)
        {
            motor_position = pull_limit_position + POSITION_TOLERANCE;
            feedback_state = PULLING;
        }
        motor.pwm = pid_calc(&motor_pid_position, motor.position, motor_position);
        HAL_Delay(LOOP_CONTROL_PERIOD);
    }
    else if (working_mode == ADJUST)
    {
        if (working_command == ADJUST_PUSHING)
        {
            motor.pwm = ADJUST_SPEED;
        }
        else if (working_command == ADJUST_PULLING)
        {
            motor.pwm = -ADJUST_SPEED;
        }
        else if (working_command == ADJUST_STOP)
        {
            motor.pwm = 0;
        }
    }
}
