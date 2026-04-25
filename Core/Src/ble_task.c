#include "ble_task.h"
#include "main.h"

void ble_task(void *argument) {
    log_record_t record;
    

    while(1) {
        xQueueReceive(xBLEQueue, &record, portMAX_DELAY);
        uint8_t pkt[12] = {
            0xAA, 0x55,
            record.co2_ppm >> 8, record.co2_ppm,
            record.pm25_ug >> 8, record.pm25_ug,
            record.aqi,
            record.temp_cdeg >> 8, record.temp_cdeg,
            record.humidity_cpct >> 8, record.humidity_cpct,
            0x00  
        };
        HAL_UART_Transmit(&huart2, pkt, 12, 100);
    }
}