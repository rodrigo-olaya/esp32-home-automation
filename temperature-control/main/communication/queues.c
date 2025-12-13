#include "queues.h"

QueueHandle_t xSensorDataQueue;

uint8_t queues_init() {
    xSensorDataQueue = xQueueCreate( 5, sizeof( sensorData_t ) );

    if( xSensorDataQueue != NULL ) {
        return 1;
    }
    return 0;
}