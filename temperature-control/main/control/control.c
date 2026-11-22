#include "control.h"
#include "../sensors/ds18b20.h"

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

    uint8_t connectedBitmap = discovery_scan(const discovery_probes_t *probes);

    /* Create the task that will read the DS18B20 data and send it to the receiver */
    xTaskCreate( ds18b20_task, "ds18b20", 4000, NULL, 1, NULL );

    /* Create the task that will read the SHT31 and send it to the receiver */
    xTaskCreate( sht31_task, "sht31", 4000, NULL, 1, NULL);

    /* Create the task that will read from the queue. The task is created
    with priority 2, so above the priority of the sender tasks. */
    xTaskCreate( vReceiverTask, "Receiver", 2048, NULL, 2, NULL );
}