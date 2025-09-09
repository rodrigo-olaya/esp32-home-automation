#include "sensors.h"

void sensor_send_data(float *temperature) {
    BaseType_t xStatus;

    xStatus = xQueueSendToBack( xSensorDataQueue, temperature, 0 );
    if( xStatus != pdPASS )
    {
        /* The send operation could not complete because the queue was full-
        this must be an error as the queue should never contain more than
        one item! */
        printf("Could not send data to the queue.\n");
    }
}