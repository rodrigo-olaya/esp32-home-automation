#include "sensors.h"

static const char* SENSORS_TAG = "QueueSend";

void sensor_send_data(float *temperature) {
    BaseType_t xStatus;

    xStatus = xQueueSendToBack( xSensorDataQueue, temperature, 0 );
    if( xStatus != pdPASS )
    {
        ESP_LOGE(SENSORS_TAG, "Could not send data to the queue");
    }
}