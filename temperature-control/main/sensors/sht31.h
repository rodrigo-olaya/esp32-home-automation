#ifndef SHT31_H
#define SHT31_H

#include <inttypes.h>
#include "../drivers/i2c_driver.h"
#include "esp_log.h"
#include "../data/handle_humidity.h"
#include "sensor_defines.h"

void sht31_init();

void sht31_task();

#endif // SHT31_H