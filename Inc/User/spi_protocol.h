#ifndef SPI_PROTOCOL
#define SPI_PROTOCOL

#include "main.h"
#include "motor_driver.h"
#include "push_pull_controller.h"

extern uint16_t spi_rx_data, spi_tx_data;

#define HEADER_ID_TEMPERATURE            0
#define HEADER_ID_POSITION               1
#define HEADER_ID_RPM                    2
#define HEADER_ID_PWM                    3
#define HEADER_ID_STATUS                 4
#define HEADER_ID_CMD                    5

#define FRAME_HEADER_LENGTH              4
#define FRAME_DATA_LENGTH                12

void spi_encode_decode();
void spi_sent();

#endif