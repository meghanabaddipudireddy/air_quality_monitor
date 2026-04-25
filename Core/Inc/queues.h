#ifndef QUEUES_H
#define QUEUES_H

#include "FreeRTOS.h"
#include "scd40.h"
#include "pms5003.h"
#include "queue.h"

extern QueueHandle_t xSensorQueue;

typedef struct {
    scd40_reading_t scd40;
    pms_reading_t   pms;
} sensor_readings_t;

#endif