#include "pms5003.h"

static uint8_t ring_buf[64];

HAL_StatusTypeDef pms5003_init(UART_HandleTypeDef *huart) {
    return HAL_UART_Receive_DMA(huart, ring_buf, 64);
}

HAL_StatusTypeDef pms5003_get_latest(UART_HandleTypeDef *huart, pms_reading_t* reading) {
    uint16_t head = 64 - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    for (int i = head - 2; i >= 0; i--) {
        if (ring_buf[i] == 0x42 && ring_buf[i+1] == 0x4D) {
            if (i + 32 > 64) continue;
            uint16_t sum = 0;
            for (int j = 0; j < 30; j++) {
                sum += ring_buf[i + j];
            }
            uint16_t checksum = (ring_buf[i + 30] << 8) | ring_buf[i + 31];
            if (sum != checksum) continue;
            
            reading->pm1_ug = (ring_buf[i+4] << 8) | ring_buf[i+5];
            reading->pm25_ug = (ring_buf[i+6] << 8) | ring_buf[i+7];
            reading->pm10_ug = (ring_buf[i+8] << 8) | ring_buf[i+9];
            return HAL_OK;
        }
    }
    return HAL_ERROR;
}