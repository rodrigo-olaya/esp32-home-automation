#include "sensors.h"

static const char* SENSORS_TAG = "QueueSend";

void sensor_send_data(float *temperature) {
    BaseType_t xStatus;

    xStatus = xQueueSendToBack( xSensorDataQueue, temperature, 0 );
    if( xStatus != pdPASS )
    {
        /* The send operation could not complete because the queue was full-
        this must be an error as the queue should never contain more than
        one item! */
        ESP_LOGE(SENSORS_TAG, "Could not send data to the queue");
    }
}