#include "control.h"
#include "../sensors/thermistor.h"
#include "../sensors/SHT_3x.h"

static const char* CONTROL_TAG = "control";

void vReceiverTask( void *pvParameters )
{
    sensorData_t sensor_data;

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
        xStatus = xQueueReceive( xSensorDataQueue, &sensor_data, xTicksToWait );
        if( xStatus == pdPASS )
        {
            ESP_LOGI(CONTROL_TAG, "Data received, calling publish_data");
            ESP_LOGI(CONTROL_TAG, "Data: %f", sensor_data.data);
            publish_data(&sensor_data);
        }
        else
        {
            ESP_LOGE(CONTROL_TAG, "Could not receive from the queue");
        }
    }
}

void sensor_send_data(sensorData_t *sensor_data) {
    BaseType_t xStatus;

    ESP_LOGI(CONTROL_TAG, "Data in send_sensor_data: %f", sensor_data->data);

    xStatus = xQueueSendToBack( xSensorDataQueue, sensor_data, 0 );
    if( xStatus != pdPASS )
    {
        ESP_LOGE(CONTROL_TAG, "Could not send data to the queue");
    }
}

void controls_init() {
    /* Create the task that will read the thermistor data and send it to the receiver */
    xTaskCreate( read_temperature, "Sender1", 4000, NULL, 1, NULL );

    /* Create the task that will read humidity and send it to the receiver*/
    xTaskCreate( read_humidity, "Sender2", 4000, NULL, 1, NULL);

    
    xTaskCreate( receive_data, "Rec1", 4000, NULL, 1, NULL);

    /* Create the task that will read from the queue. The task is created
    with priority 2, so above the priority of the sender tasks. */
    xTaskCreate( vReceiverTask, "Receiver", 2048, NULL, 2, NULL );
}