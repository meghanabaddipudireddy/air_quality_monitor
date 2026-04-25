#include "scd40.h"

HAL_StatusTypeDef scd40_init(I2C_HandleTypeDef *hi2c) {
    uint8_t cmd[2] = {0x21, 0xB1};
    return HAL_I2C_Master_Transmit(hi2c, 0x62 << 1, cmd, 2, 100);
}

static uint8_t scd40_crc(uint8_t *data, uint16_t len) {
    uint8_t crc = 0xFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc <<= 1;
        }
    }
    return crc;
}

HAL_StatusTypeDef scd40_read(I2C_HandleTypeDef *hi2c, scd40_reading_t* reading) {
    uint8_t cmd[2] = {0xEC, 0x05};
    if (HAL_I2C_Master_Transmit(hi2c, 0x62 << 1, cmd, 2, 100) != HAL_OK) {
        return HAL_ERROR;
    }
    HAL_Delay(1);
    uint8_t buf[9];
    if (HAL_I2C_Master_Receive(hi2c, 0x62 << 1, buf, 9, 100) != HAL_OK) {
        return HAL_ERROR;
    }
    if (scd40_crc(buf, 2) != buf[2]) {
        return HAL_ERROR;
    }
    if (scd40_crc(buf+3, 2) != buf[5]) {
        return HAL_ERROR;
    }
    if (scd40_crc(buf+6, 2) != buf[8]) {
        return HAL_ERROR;
    }
    reading->co2_ppm = (buf[0] << 8) | buf[1];
    reading->temp_cdeg = (buf[3] << 8) | buf[4];
    reading->humidity_cpct = (buf[6] << 8) | buf[7];

    return HAL_OK;
}