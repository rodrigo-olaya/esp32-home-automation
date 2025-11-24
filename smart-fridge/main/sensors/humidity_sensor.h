#ifndef HUMIDITY_SENSOR_H
#define HUMIDITY_SENSOR_H

#include <inttypes.h>
#include "../drivers/i2c_driver.h"
#include "esp_log.h"

void humidity_sensor_initialize();

#endif // HUMIDITY_SENSOR_H