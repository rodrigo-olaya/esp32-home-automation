#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "onewire.h"
#include "ds18x20.h"
#include "esp_log.h"
#include <inttypes.h> 
#include <portmacro.h>
#include "../communication/sensors.h"
#include "../communication/gpio_config.h"

void read_temperature (void *pvParameters);