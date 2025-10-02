#include "control.h"
#include "../sensors/thermistor.h"
#include "../sensors/SHT_3x.h"

static const char* CONTROL_TAG = "control";

void vReceiverTask( void *pvParameters )
{
    /* Declare the variable that will hold the values received from the queue. */
    // sensorType_t sensor_type; = sensor_data->type;
    // float sensor_data; = sensor_data->type;

    sensorData_t sensor_data;
    
    // float lReceivedValue;
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
            // sensorType_t sensor_type = sensor_data.type;
            // float data = sensor_data.data;

            ESP_LOGI(CONTROL_TAG, "Data received, calling publish_data");
            // ESP_LOGI(CONTROL_TAG, "Data received from queue: %f", lReceivedValue);
            publish_data(sensor_data);
        }
        else
        {
            ESP_LOGE(CONTROL_TAG, "Could not receive from the queue");
        }
    }
}

void sensor_send_data(sensorData_t *sensor_data) {
    BaseType_t xStatus;

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

    /* Create the task that will read from the queue. The task is created
    with priority 2, so above the priority of the sender tasks. */
    xTaskCreate( vReceiverTask, "Receiver", 2048, NULL, 2, NULL );
}