#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "onewire.h"
#include "ds18x20.h"
#include <portmacro.h>
#include "freertos/queue.h"
#include "../communication/queues.h"

extern QueueHandle_t xSensorDataQueue;

void vReceiverTask( void *pvParameters );