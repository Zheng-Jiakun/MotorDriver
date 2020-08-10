#include "spi_protocol.h"

uint16_t spi_rx_data, spi_tx_data;

void spi_sent()
{
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    // HAL_Delay(1);
    HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&spi_tx_data, (uint8_t *)&spi_rx_data, 1, 100);
    // HAL_Delay(1);
    HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void spi_encode()
{
    static uint8_t frame_count = 0;
    switch (frame_count)
    {
    case 0:
        spi_tx_data = HEADER_ID_POSITION << FRAME_DATA_LENGTH | ((motor.position - pull_limit_position) & 0xfff);
        break;

    case 1:
        spi_tx_data = HEADER_ID_TEMPERATURE << FRAME_DATA_LENGTH | ((int16_t)(motor.temperature * 10) & 0xfff);
        break;

    case 2:
        spi_tx_data = HEADER_ID_RPM << FRAME_DATA_LENGTH | ((motor.rpm + 2048) & 0xfff);
        break;

    case 3:
        spi_tx_data = HEADER_ID_STATUS << FRAME_DATA_LENGTH | (feedback_state & 0xfff);
        break;

    default:
        break;
    }
    frame_count++;
    if (frame_count > 3)
        frame_count = 0;
}

void spi_decode()
{
    uint8_t header = (spi_rx_data >> FRAME_DATA_LENGTH) & 0xf;
    uint16_t data = spi_rx_data & 0xfff;

    switch (header)
    {
    case HEADER_ID_SPEED:
        speed_control = data;
        break;

    case HEADER_ID_MODE:
        working_mode = data;
        break;

    case HEADER_ID_CMD:
        working_command = data;
        break;

    default:
        break;
    }
}

void spi_encode_decode()
{
    spi_encode();
    spi_decode();
    spi_sent();
}
