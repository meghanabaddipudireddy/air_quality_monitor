#ifndef PMS5003_H
#define PMS5003_H
#include "stm32l4xx_hal.h"
#include <stdint.h>

typedef struct {
    uint16_t pm1_ug;
    uint16_t pm25_ug;
    uint16_t pm10_ug;
} pms_reading_t;

HAL_StatusTypeDef pms5003_init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef pms5003_get_latest(UART_HandleTypeDef *huart, pms_reading_t* reading);

#endif