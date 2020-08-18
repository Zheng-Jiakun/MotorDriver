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

uint8_t spi_get_parity(uint16_t spi_data)
{
    uint8_t cnt = 0;
    while (spi_data)
    {
        if (spi_data & 0x0001)
            cnt++;
        spi_data >>= 1;
    }
    return cnt % 2;
}

void spi_encode()
{
    spi_tx_data = 0x0000;
    static uint8_t frame_count = 0;
    switch (frame_count)
    {
    case 0:
        spi_tx_data = HEADER_ID_POSITION << FRAME_DATA_LENGTH | ((uint16_t)((float)(motor.position - pull_limit_position) / (float)(push_limit_position - pull_limit_position) * 1000) & 0xfff);
        break;

    case 1:
        spi_tx_data = HEADER_ID_TEMPERATURE << FRAME_DATA_LENGTH | ((uint16_t)(motor.temperature * 10) & 0xfff);
        break;

    case 2:
        spi_tx_data = HEADER_ID_RPM << FRAME_DATA_LENGTH | (testing_rpm & 0xfff);
        break;

    case 3:
        spi_tx_data = HEADER_ID_STATUS << FRAME_DATA_LENGTH | (feedback_state & 0xfff);
        break;

    case 4:
        spi_tx_data = HEADER_ID_ERROR << FRAME_DATA_LENGTH | (motor.error & 0xfff);
        break;

    default:
        break;
    }
    frame_count++;
    if (frame_count > 4)
        frame_count = 0;

    if (spi_get_parity(spi_tx_data) == 0)
        spi_tx_data |= 1U << (FRAME_DATA_LENGTH + FRAME_HEADER_LENGTH);
}

void spi_decode()
{
    if (spi_get_parity(spi_rx_data) == 1)
    {
        uint8_t header = (spi_rx_data >> FRAME_DATA_LENGTH) & 0x7;
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
        spi_rx_data = 0x0000;
    }
}

void spi_timeout_handler()
{
    static uint8_t spi_error_cnt = 0;

    if (spi_rx_data == 0x0000)
        spi_error_cnt++;
    else
        spi_error_cnt = 0;

    if (spi_error_cnt > SPI_TIMEOUT)
    {
        speed_control = SPEED_HOLD;
        working_mode = AUTO;
        working_command = AUTO_STOP;
    }
}

void spi_encode_decode()
{
    spi_encode();
    spi_timeout_handler();
    spi_decode();
    spi_sent();
}
