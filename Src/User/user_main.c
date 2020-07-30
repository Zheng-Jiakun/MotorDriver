#include "user_main.h"
#include "motor_driver.h"
#include "algorithm.h"
#include "push_pull_controller.h"

pid_t motor_pid;

void user_setup()
{
    motor_start();
    push_pull_init();
    
    // HAL_TIM_Base_Start_IT(&htim4);
}

void user_loop()
{
    testing_thread();
    // normal_testing();
    // position_calibrate();
}