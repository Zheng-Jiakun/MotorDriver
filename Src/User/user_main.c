#include "user_main.h"
#include "motor_driver.h"
#include "algorithm.h"
#include "push_pull_controller.h"
#include "spi_protocol.h"

void user_setup()
{
    motor_start();
    push_pull_init();
}

void user_loop()
{
    testing_thread();
    error_handler();
}