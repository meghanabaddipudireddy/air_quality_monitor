#ifndef EINK_H
#define EINK_H

#include "w25q32.h"

HAL_StatusTypeDef eink_init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef eink_display(SPI_HandleTypeDef *hspi, log_record_t* logs);


#endif