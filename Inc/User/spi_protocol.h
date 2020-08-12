#ifndef SPI_PROTOCOL
#define SPI_PROTOCOL

#include "main.h"
#include "motor_driver.h"
#include "push_pull_controller.h"

extern uint16_t spi_rx_data, spi_tx_data;

#define HEADER_ID_TEMPERATURE            0
#define HEADER_ID_POSITION               1
#define HEADER_ID_RPM                    2
#define HEADER_ID_SPEED                  3
#define HEADER_ID_STATUS                 4
#define HEADER_ID_MODE                   5
#define HEADER_ID_CMD                    6
#define HEADER_ID_ERROR                  7

#define FRAME_HEADER_LENGTH              3
#define FRAME_DATA_LENGTH                12

#define SPI_TIMEOUT                      100

void spi_encode_decode();
void spi_sent();
void spi_timeout_handler();

#endif