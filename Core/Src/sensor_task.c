#include "sensor_task.h"
#include "main.h"


void sensor_task(void *argument) {
    sensor_readings_t reading;

    while(1) {
        
        if (scd40_read(&hi2c1, &reading.scd40) != HAL_OK) continue;
        if (pms5003_get_latest(&huart3, &reading.pms) != HAL_OK) continue;
        xQueueSend(xSensorQueue, &reading, 0);
        vTaskDelay(pdMS_TO_TICKS(5000));

    }
}