#ifndef W25Q32_H
#define W25Q32_H
#include "stm32l4xx_hal.h"
#include <stdint.h>

typedef struct {
    uint16_t co2_ppm;
    int16_t temp_cdeg;
    uint16_t humidity_cpct;
    uint16_t pm25_ug;
    uint16_t aqi;
    uint32_t time;
} log_record_t;

HAL_StatusTypeDef w25q32_init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef w25q32_write_record(SPI_HandleTypeDef *hspi, log_record_t* logs);
HAL_StatusTypeDef w25q32_read_record(SPI_HandleTypeDef *hspi,uint32_t record, log_record_t* logs);

#endif