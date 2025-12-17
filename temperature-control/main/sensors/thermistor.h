#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "onewire.h"
// #include "ds18x20.h"
#include "esp_log.h"
#include <inttypes.h> 
#include <portmacro.h>
#include "../drivers/gpio_config.h"
#include "../control/control.h"
#include "sensor_defines.h"
#include "../data/handle_temperature.h"
#include "../drivers/onewire.h"

// /**
//  * @brief This function reads the temperature from the DS18B20 and calls the sensor control function with the function.
//  * @param pvParameters Parameters needed my vTask creation
//  */
// void read_temperature (void *pvParameters);

void test_onewire();

#endif // THERMISTOR_H