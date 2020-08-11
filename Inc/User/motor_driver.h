#ifndef MOTOR_DRIVER
#define MOTOR_DRIVER

#define PWM_DUTYCYCLE2PULSE(x)      ((float)(x)/100.0f*4500)

#include "main.h"
#include "tim.h"
#include "spi.h"
#include "adc.h"
#include "algorithm.h"
#include "user_main.h"
#include <stdlib.h>
#include <math.h>


extern uint32_t hall_tick_10us;

#define ADC_MEANFILTER_SIZE         10
#define SPEED_FILTER_SIZE           9
#define ZERO_SPEED_TIMEOUT          5000U

#define DRV8301_MODE_W              0
#define DRV8301_MODE_R              1
#define DRV8301_ADDRESS_STATUS0     0x00
#define DRV8301_ADDRESS_STATUS1     0x01
#define DRV8301_ADDRESS_CONTROL1    0x02
#define DRV8301_ADDRESS_CONTROL2    0x03

#define DRV8301_RW_BIT_OFFSET       15
#define DRV8301_ADDRESS_BIT_OFFSET  11

#define DRV8301_GET_SPI_FRAME(rw, address, data)   ((rw) << DRV8301_RW_BIT_OFFSET) | ((address) << DRV8301_ADDRESS_BIT_OFFSET | (data))

#define DRV8301_CURRENT_AMP_GAIN    80
#define CURRENT_SAMPLE_RES          0.002f
#define ADC2VOLTAGE(x)              ((x)/4095.0f*3.3f)
#define CURRENT_LINEAR_OFFSET       -0.0f
#define DRV8301_VOLTAGE2CURRENT(x)  ((1.65f-(x))/(float)DRV8301_CURRENT_AMP_GAIN/(CURRENT_SAMPLE_RES)+CURRENT_LINEAR_OFFSET)
// #define DRV8301_VOLTAGE2CURRENT(x)  (-10.068f*(x)+16.037+CURRENT_LINEAR_OFFSET)
// #define DRV8301_VOLTAGE2CURRENT(x)  (-5.2726f*(x)+8.4632+CURRENT_LINEAR_OFFSET)

#define ADC_CHANNEL_NUM             2
#define ADC_CURRENT_CHANNEL         0
#define ADC_TEMPERATURE_CHANNEL     1

#define ADC2RES(x) ((x)*1e4 / (4095 - (x)))
#define RES2TEMP(x) (3435.0f / ((3435.0f / 298.15f) - log(1e4 / (x))) - 273.15f)

#define HALL2DEGREE(x)              (360.0f/42.0f*(x))
#define DEGREE2HALL(x)              ((x)*42.0f/360.0f)

#define LOOP_CONTROL_PERIOD         50

typedef enum {
    HIGH_ON_LOW_OFF = 0,
    HIGH_OFF_LOW_ON,
    HIGH_OFF_LOW_OFF
} mosfet_state_t;

typedef struct {
    int8_t pwm;
    int16_t rpm;
    float current;
    int32_t position;
    float degree;
    float temperature;
} motor_t;

extern pid_t motor_pid_current, motor_pid_speed, motor_pid_position;
extern motor_t motor;

void motor_start();
void motor_switch_phase();
void motor_get_speed();
void motor_check_0_speed();
void motor_get_current();
void motor_get_temperature();
void motor_get_position();
void motor_current_loop(float set);
void motor_speed_loop(float set);
void motor_position_loop(float set);
void adc_filter();
void motor_adjust_pwm();
#endif