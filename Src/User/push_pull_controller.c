#include "push_pull_controller.h"

float middle_ratio = 0.6f;

pid_t motor_pid_speed, motor_pid_position;

uint8_t calibrated_flag = 0;
int16_t push_limit_position, pull_limit_position;

int16_t testing_rpm = TESTING_RPM;
static int16_t motor_rpm = 0;

working_mode_t working_mode;
feedback_state_t feedback_state;
working_command_t working_command;
speed_control_t speed_control;

int16_t motor_position = 0;

void push_pull_init()
{

    // pid_init(&motor_pid_current, 5, 50, 50, 50.0f, 5.0f, 0.5f);
    // pid_init(&motor_pid_speed, 0, 50, 50, 0.03f, 0.01f, 0.005f);
    // pid_init(&motor_pid_position, 400, 1200, 1000, 1.0f, 0.0f, 0.0f);

    pid_init(&motor_pid_speed, 0, 80, 80, 0.008f, 0.001f, 0.0008f);
    pid_init(&motor_pid_position, -40, 40, 50, 0.15f, 0.0001f, 0.01f);

    motor.pwm = CALIBRATING_SPEED;
}

void get_middle_ratio()
{
    if (motor.position - pull_limit_position > (push_limit_position - pull_limit_position) * MIDDLE_POSITION_PUSH)
        middle_ratio = MIDDLE_POSITION_PULL;
    else if (motor.position - pull_limit_position < (push_limit_position - pull_limit_position) * MIDDLE_POSITION_PULL)
        middle_ratio = MIDDLE_POSITION_PUSH;
    // else
    //     middle_ratio = (MIDDLE_POSITION_PULL + MIDDLE_POSITION_PUSH) / 2.0f;
}

void position_calibrate()
{
    static uint8_t stuck_flag = 0, zero_rpm_flag = 0;
    static int8_t direction = 0;
    static uint32_t stuck_tick = -1;

    if (abs(motor.rpm) == 0 && zero_rpm_flag == 0)
    {
        stuck_tick = HAL_GetTick();
        zero_rpm_flag = 1;
    }
    else if (abs(motor.rpm) > 0)
    {
        zero_rpm_flag = 0;
    }
    else if (HAL_GetTick() - stuck_tick > ZERO_RPM_TIMEOUT && HAL_GetTick() - stuck_tick < ZERO_RPM_TIMEOUT + ZERO_SPEED_GAP && zero_rpm_flag == 1 && stuck_flag == 0)
    {
        if (motor.pwm < 0)
        {
            direction = -1;
            pull_limit_position = motor.position;
        }
        else if (motor.pwm > 0)
        {
            direction = 1;
            push_limit_position = motor.position;
        }
        motor.pwm = 0;

        stuck_flag = 1;
    }
    else if (HAL_GetTick() - stuck_tick > ZERO_RPM_TIMEOUT + ZERO_SPEED_GAP && stuck_flag == 1)
    {
        stuck_flag = 0;
        if (direction == 1)
            motor.pwm = -CALIBRATING_SPEED;
        else if (direction == -1)
            motor.pwm = CALIBRATING_SPEED;
        // motor_change_phase();
    }

    if (push_limit_position - pull_limit_position > POSITION_DIFFERENCE)
    {
        // motor.pwm = 0;
        calibrated_flag = 1;
        // get_middle_ratio();
        // motor_position = pull_limit_position + (push_limit_position - pull_limit_position) * middle_ratio;
    }
}

void normal_testing()
{
    static uint8_t stuck_flag = 0;

    if (motor.position > (push_limit_position - POSITION_TOLERANCE) && stuck_flag == 0)
    {
        // motor.pwm = -running_pwm;
        motor_rpm = -testing_rpm;
        // motor_position = pull_limit_position;
        stuck_flag = 1;
    }
    else if ((motor.position < pull_limit_position + POSITION_TOLERANCE) && stuck_flag == 0)
    {
        // motor.pwm = running_pwm;
        motor_rpm = testing_rpm;
        // motor_position = push_limit_position;
        stuck_flag = 1;
    }
    else if (motor.position < (push_limit_position - 2 * POSITION_TOLERANCE) && motor.position > (pull_limit_position + 2 * POSITION_TOLERANCE))
    {
        stuck_flag = 0;
    }

    if (motor_rpm >= 0)
        motor.pwm = pid_calc(&motor_pid_speed, motor.rpm, motor_rpm);
    else
        motor.pwm = -pid_calc(&motor_pid_speed, -motor.rpm, -motor_rpm);

    // motor.pwm = pid_calc(&motor_pid_position, motor.position, motor_position);

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
        static uint8_t start_flag = 0;
        if (working_command == AUTO_PUSHING || working_command == AUTO_PULLING)
        {
            feedback_state = RUNNING;
            if (start_flag == 0)
            {
                if (working_command == AUTO_PUSHING)
                    motor_rpm = testing_rpm;
                else if (working_command == AUTO_PULLING)
                    motor_rpm = -testing_rpm;
                start_flag = 1;
            }
            normal_testing();
        }
        else if (working_command == AUTO_STOP)
        {
            feedback_state = STOP;
            motor.pwm = 0;
            start_flag = 0;
            HAL_Delay(LOOP_CONTROL_PERIOD);
        }

        // if (speed_control == SPEED_UP)
        // {
        //     if (running_pwm < 60)
        //         running_pwm++;
        //     if (motor.pwm > 0 && motor.pwm < running_pwm)
        //         motor.pwm++;
        // }
        // else if (speed_control == SPEED_DOWN)
        // {
        //     if (running_pwm > 3)
        //         running_pwm--;
        //     if (motor.pwm < 0 && motor.pwm > -running_pwm)
        //         motor.pwm--;
        // }

        int8_t sign = motor_rpm > 0 ? 1 : -1;
        int16_t temp_rpm = abs(motor_rpm);

        if (speed_control == SPEED_UP)
        {
            if (testing_rpm < MAX_RPM)
                testing_rpm += RPM_STEP;

            if (temp_rpm < testing_rpm)
            {
                temp_rpm += RPM_STEP;
                motor_rpm = temp_rpm * sign;
            }
            // HAL_Delay(20);
        }
        else if (speed_control == SPEED_DOWN)
        {
            if (testing_rpm > MIN_RPM)
                testing_rpm -= RPM_STEP;

            if (temp_rpm > MIN_RPM)
            {
                temp_rpm -= RPM_STEP;
                motor_rpm = temp_rpm * sign;
            }
            // HAL_Delay(20);
        }
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
            get_middle_ratio();
            motor_position = pull_limit_position + (push_limit_position - pull_limit_position) * middle_ratio;
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
            feedback_state = PUSHING;
        }
        else if (working_command == ADJUST_PULLING)
        {
            motor.pwm = -ADJUST_SPEED;
            feedback_state = PULLING;
        }
        else if (working_command == ADJUST_STOP)
        {
            motor.pwm = 0;
            feedback_state = STOP;
        }
    }
}
