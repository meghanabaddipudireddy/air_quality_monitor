
#ifndef SCD40_H
#define SCD40_H
#include "stm32l4xx_hal.h"
#include <stdint.h>

typedef struct {
    uint16_t co2_ppm;
    int16_t temp_cdeg;
    uint16_t humidity_cpct;
} scd40_reading_t;

HAL_StatusTypeDef scd40_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef scd40_read(I2C_HandleTypeDef *hi2c, scd40_reading_t* reading);

#endif
