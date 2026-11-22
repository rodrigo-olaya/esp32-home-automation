#include "control.h"
#include "../sensors/ds18b20.h"
#include "esp_timer.h"

static const char* CONTROL_TAG = "control";
static discovery_health_t health;

/* Task handles, so stop() can delete the task that start() created. NULL means "no task". */
static TaskHandle_t sht31_handle = NULL;
static TaskHandle_t ds18b20_handle = NULL;


void vReceiverTask( void *pvParameters )
{
    sensorData_t sensor_data;

    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS( 100000 );

    for( ;; )
    {
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

void start(sensor_flag_t sensor) {
    if (sensor == SENSOR_SHT31) {
        /* Create the task that will read the SHT31 and send it to the receiver */
        xTaskCreate( sht31_task, "sht31", 4000, NULL, 1, &sht31_handle);
    }
    else if (sensor == SENSOR_DS18B20) {
        /* Create the task that will read the DS18B20 data and send it to the receiver */
        xTaskCreate( ds18b20_task, "ds18b20", 4000, NULL, 1, &ds18b20_handle );
    }
}

void stop(sensor_flag_t sensor) {
    if (sensor == SENSOR_SHT31 && sht31_handle != NULL) {
        /* Holding the I2C lock while deleting means the task is not in the middle of a transaction,
         * so it cannot die holding the lock. We wait here until it finishes the one it is in. */
        i2c_lock();
        vTaskDelete(sht31_handle);
        i2c_unlock();
        sht31_handle = NULL;
    }
    else if (sensor == SENSOR_DS18B20 && ds18b20_handle != NULL) {
        vTaskDelete(ds18b20_handle);
        ds18b20_handle = NULL;
    }
}

void mark_sensor_seen(sensor_flag_t sensor) {
    if (sensor == SENSOR_SHT31){
        health.sht31.last_seen_ms = (uint32_t)(esp_timer_get_time() / 1000);
    }
    if (sensor == SENSOR_DS18B20){
        health.ds18b20.last_seen_ms = (uint32_t)(esp_timer_get_time() / 1000);
    }
}

void discovery_task() {
    discovery_state_t state;
    discovery_probes_t probes;
    
    discovery_actions_t actions;

    state.running = 0;
    probes.sht31 = sht31_probe;
    probes.ds18b20 = ds18b20_probe;

    sensor_health_t sht32_health;
    sensor_health_t ds18b20_health;

    sht32_health.interval_ms = SHT31_INTERVAL_MS;
    ds18b20_health.interval_ms = DS18B20_INTERVAL_MS;

    sht32_health.last_seen_ms = 0;
    ds18b20_health.last_seen_ms = 0;

    health.sht31 = sht32_health;
    health.ds18b20 = ds18b20_health;

    actions.start = start;
    actions.stop = stop;

    for ( ;; ) {
        discovery_step(&state, &probes, &health, (uint32_t)(esp_timer_get_time() / 1000), &actions);
        vTaskDelay(pdMS_TO_TICKS(DISCOVERY_PERIOD_MS));
    }
}

void controls_init() {

    xTaskCreate( discovery_task, "discovery", 4000, NULL, 2, NULL);
    
    /* Create the task that will read from the queue. The task is created
    with priority 2, so above the priority of the sender tasks. */
    xTaskCreate( vReceiverTask, "Receiver", 4096, NULL, 2, NULL );
}