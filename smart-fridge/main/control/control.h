#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../MQTT/publisher.h"
#include "../sensors/sensor_defines.h"

extern QueueHandle_t xSensorDataQueue;

void sensor_send_data(sensorData_t *sensor_data);

void vReceiverTask( void *pvParameters );

void controls_init();