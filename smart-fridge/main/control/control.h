#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
# include "../sensors/thermistor.h"

extern QueueHandle_t xSensorDataQueue;

void vReceiverTask( void *pvParameters );

void controls_init();