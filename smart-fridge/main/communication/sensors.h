#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

extern QueueHandle_t xSensorDataQueue;

typedef enum {
    TEMPERATURE,
    HUMIDITY
} sensorType_t;

typedef struct {
    sensorType_t type;
    float data;
} sensorData_t;

void sensor_send_data();