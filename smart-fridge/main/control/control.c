#include "control.h"

static const char* CONTROL_TAG = "control";

void vReceiverTask( void *pvParameters )
{
    /* Declare the variable that will hold the values received from the queue. */
    float lReceivedValue;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS( 100000 );

    for( ;; )
    {
        /* This call should always find the queue empty */
        if( uxQueueMessagesWaiting( xSensorDataQueue ) != 0 )
        {
            ESP_LOGE(CONTROL_TAG, "Queue should have been empty");
        }
        /* Receive data from the queue. */
        xStatus = xQueueReceive( xSensorDataQueue, &lReceivedValue, xTicksToWait );
        if( xStatus == pdPASS )
        {
            ESP_LOGI(CONTROL_TAG, "Data received, calling publish_data");
            publish_data(lReceivedValue);
        }
        else
        {
            ESP_LOGE(CONTROL_TAG, "Could not receive from the queue");
        }
    }
}

void controls_init() {
    /* Create the task that will read the thermistor data and send it to the receiver */
    xTaskCreate( read_temperature, "Sender1", 4000, NULL, 1, NULL );
    /* Create the task that will read from the queue. The task is created
    with priority 2, so above the priority of the sender tasks. */
    xTaskCreate( vReceiverTask, "Receiver", 2048, NULL, 2, NULL );
}