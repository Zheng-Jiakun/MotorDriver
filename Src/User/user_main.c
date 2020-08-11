#include "user_main.h"
#include "motor_driver.h"
#include "algorithm.h"
#include "push_pull_controller.h"
#include "spi_protocol.h"

pid_t motor_pid;

void user_setup()
{
    motor_start();
    push_pull_init();

    // motor_position = 300;
}

void user_loop()
{
    // motor.pwm = pid_calc(&motor_pid_position, motor.position, motor_position);
    // HAL_Delay(LOOP_CONTROL_PERIOD);

    testing_thread();
    // normal_testing();
    // position_calibrate();
}