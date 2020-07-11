#include "motor_driver.h"

uint16_t speed_rpm = 0;

uint8_t read_hall()
{
    uint8_t hall = 0b000;
    if (HAL_GPIO_ReadPin(HALL_A_GPIO_Port, HALL_A_Pin) == GPIO_PIN_SET)
        hall |= 0b001;
    if (HAL_GPIO_ReadPin(HALL_B_GPIO_Port, HALL_B_Pin) == GPIO_PIN_SET)
        hall |= 0b010;
    if (HAL_GPIO_ReadPin(HALL_C_GPIO_Port, HALL_C_Pin) == GPIO_PIN_SET)
        hall |= 0b100;

    return hall;
}

void mosfet_control(uint8_t n, mosfet_state_t s)
{
    switch (n)
    {
    case 1:
        if (s == HIGH_OFF_LOW_ON)
        {
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM_DUTYCYCLE2PULSE(100));
        }
        else if (s == HIGH_ON_LOW_OFF)
        {
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWM_DUTYCYCLE2PULSE(100 - PWM_DUTYCYCLE));
        }
        else if (s == HIGH_OFF_LOW_OFF)
        {
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
        }
        break;

    case 2:
        if (s == HIGH_OFF_LOW_ON)
        {
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM_DUTYCYCLE2PULSE(100));
        }
        else if (s == HIGH_ON_LOW_OFF)
        {
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM_DUTYCYCLE2PULSE(100 - PWM_DUTYCYCLE));
        }
        else if (s == HIGH_OFF_LOW_OFF)
        {
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
        }
        break;

    case 3:
        if (s == HIGH_OFF_LOW_ON)
        {
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWM_DUTYCYCLE2PULSE(100));
        }
        else if (s == HIGH_ON_LOW_OFF)
        {
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWM_DUTYCYCLE2PULSE(100 - PWM_DUTYCYCLE));
        }
        else if (s == HIGH_OFF_LOW_OFF)
        {
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
        }
        break;

    default:
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        break;
    }
}

void phase_select(uint8_t phase)
{
    switch (phase)
    {
    case 0:
        mosfet_control(1, HIGH_OFF_LOW_OFF);
        mosfet_control(2, HIGH_OFF_LOW_OFF);
        mosfet_control(3, HIGH_OFF_LOW_OFF);
        break;

    case 5://AB
        mosfet_control(1, HIGH_ON_LOW_OFF);
        mosfet_control(2, HIGH_OFF_LOW_ON);
        mosfet_control(3, HIGH_OFF_LOW_OFF);
        break;

    case 4://CB
        mosfet_control(1, HIGH_OFF_LOW_OFF);
        mosfet_control(2, HIGH_OFF_LOW_ON);
        mosfet_control(3, HIGH_ON_LOW_OFF);
        break;

    case 6://CA
        mosfet_control(1, HIGH_OFF_LOW_ON);
        mosfet_control(2, HIGH_OFF_LOW_OFF);
        mosfet_control(3, HIGH_ON_LOW_OFF);
        break;

    case 2://BA
        mosfet_control(1, HIGH_OFF_LOW_ON);
        mosfet_control(2, HIGH_ON_LOW_OFF);
        mosfet_control(3, HIGH_OFF_LOW_OFF);
        break;

    case 3://BC
        mosfet_control(1, HIGH_OFF_LOW_OFF);
        mosfet_control(2, HIGH_ON_LOW_OFF);
        mosfet_control(3, HIGH_OFF_LOW_ON);
        break;

    case 1://AC
        mosfet_control(1, HIGH_ON_LOW_OFF);
        mosfet_control(2, HIGH_OFF_LOW_OFF);
        mosfet_control(3, HIGH_OFF_LOW_ON);
        break;

    default:
        break;
    }
}

void motor_start()
{
    while (read_hall() == 0b000 || read_hall() == 0b111)
        ;

    HAL_GPIO_WritePin(DRV8301_ENGATE_GPIO_Port, DRV8301_ENGATE_Pin, GPIO_PIN_SET);

    motor_change_phase();
}

void motor_stop()
{
    phase_select(0);

    HAL_GPIO_WritePin(DRV8301_ENGATE_GPIO_Port, DRV8301_ENGATE_Pin, GPIO_PIN_RESET);
}

void motor_change_phase()
{
    phase_select(read_hall());
}

void get_speed()
{
    static uint32_t last_tick = 0;
    static uint8_t filter_cnt = 0;
    static uint16_t speed_rpm_tmp = 0;
    speed_rpm_tmp += 60.0f / ((HAL_GetTick() - last_tick) / 1000.0f * 14.0f * 3.0f);
    if (filter_cnt >= 100)
    {
        speed_rpm = speed_rpm_tmp / 100;
        speed_rpm_tmp = 0;
        filter_cnt = 0;
    }
    
    filter_cnt++;
    last_tick = HAL_GetTick();
}