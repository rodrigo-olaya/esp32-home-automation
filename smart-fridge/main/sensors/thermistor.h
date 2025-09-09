#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "onewire.h"
#include "ds18x20.h"
#include <portmacro.h>
#include "../communication/sensors.h"

#define GPIO_NUM 4

void read_temperature (void *pvParameters);