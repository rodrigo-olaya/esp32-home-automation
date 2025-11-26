#ifndef HUMIDITY_SENSOR_H
#define HUMIDITY_SENSOR_H

#include <inttypes.h>
#include "../drivers/i2c_driver.h"
#include "esp_log.h"
#include "../data/handle_humidity.h"
#include "sensor_defines.h"
#include "../control/control.h"

void humidity_sensor_initialize();

void read_humidity_new();

#endif // HUMIDITY_SENSOR_H