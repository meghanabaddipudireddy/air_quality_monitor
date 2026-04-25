#include "pipeline_task.h"

static uint16_t calculate_aqi(uint16_t pm25) {
    if (pm25 <= 12) {
        return (uint16_t)(((float)(50 - 0) / (12 - 0)) * (pm25 - 0) + 0);
    } else if (pm25 <= 35) {
        return (uint16_t)(((float)(100 - 51) / (35 - 12)) * (pm25 - 12) + 51);
    } else if(pm25 <= 55) {
        return (uint16_t)(((float)(150-101)/(55-35)) * (pm25 - 35) + 101);
    } else if (pm25 <= 150) {
        return (uint16_t)(((float)(200-151)/(150-55)) * (pm25 - 55) + 151);
    } else if (pm25 <= 250) {
        return (uint16_t)(((float)(300-201)/(250-151)) * (pm25-151) + 201);
    } else if (pm25 > 250) {
        return (uint16_t)(((float)(500-301)/(pm25 - 250)) * (pm25 - 250) + 301);
    }
}


void pipeline_task(void *argument) {
    sensor_readings_t reading;
    log_record_t record;

    while(1) {
        xQueueReceive(xSensorQueue, &reading, portMAX_DELAY);
        record.aqi = calculate_aqi(reading.pms.pm25_ug);
        record.co2_ppm = reading.scd40.co2_ppm;
        record.temp_cdeg = reading.scd40.temp_cdeg;
        record.humidity_cpct = reading.scd40.humidity_cpct;
        record.pm25_ug = reading.pms.pm25_ug;
        xQueueSend(xFlashQueue, &record, 0);
        xQueueSend(xBLEQueue, &record, 0);
    }
}