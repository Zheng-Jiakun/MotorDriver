#include "push_pull_controller.h"

uint8_t calibrated_flag = 0;

void position_calibrate ()
{
    // static uint32_t stuck_position = 0, stuck_start_tick = 0;
    // static uint8_t stuck_start_flag = 0;
    // if (abs(motor.position - stuck_position) < 3 && stuck_start_flag == 0)
    // {
    //     stuck_start_flag = 1;
    //     stuck_start_tick = HAL_GetTick();
    // }
    // else if (HAL_GetTick() - stuck_start_tick > 100 && stuck_start_flag == 1)
    // {
    //     motor.pwm = -motor.pwm;
    //     stuck_start_flag = 0;
    // }
    // else
    // {
    //     stuck_start_flag = 0;
    // }
    // stuck_position == motor.position;


    // static uint32_t last_position = 0;
    static uint8_t stuck_flag = 0;
    // if (abs(last_position - motor.position) < 30 && stuck_flag == 0)
    // {
    //     motor.pwm = -motor.pwm;
    //     stuck_flag = 1;
    // }
    // if (stuck_flag == 1 && abs(motor.rpm) > 30)
    // {
    //     stuck_flag = 0;
    // }
    // last_position = motor.position;
    // HAL_Delay(500);

    // if (abs(motor.rpm) < 10 && stuck_flag == 0)
    // {
    //     motor.pwm = -motor.pwm;
    //     stuck_flag = 1;
    // }
    // else if (abs(motor.rpm > 10))
    // {
    //     stuck_flag = 0;
    // }

    if (motor.current > 2.6f && stuck_flag == 0)
    {
        motor.pwm = -motor.pwm;
        stuck_flag = 1;
    }
    else if (motor.current < 1.0f)
    {
        stuck_flag = 0;
    }

    /*
    12v
    pwm 10 => 0.6 ~ 0.9
    pwm 20 => 0.8 ~ 1.5
    pwm 30 => 1.0 ~ 2.6
    pwm 40 => 
    */
}
