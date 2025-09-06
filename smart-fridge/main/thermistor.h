#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "onewire.h"
#include "ds18x20.h"
#include <portmacro.h>
#include "freertos/queue.h"

#define GPIO_NUM 4

// Straight form FreeRTOS book
extern QueueHandle_t xQueue;

void read_temperature (void *pvParameters);