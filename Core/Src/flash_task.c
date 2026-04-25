#include "flash_task.h"
#include "main.h"

void flash_task(void *argument) {
    log_record_t record;

    while(1) {
        xQueueReceive(xFlashQueue, &record, portMAX_DELAY);
        if(w25q32_write_record(&hspi1, &record) != HAL_OK) continue;
    }
}