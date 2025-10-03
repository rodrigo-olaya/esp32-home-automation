#include <sht3x.h>
#include "esp_log.h"
#include "../communication/gpio_config.h"
#include "sensor_defines.h"
#include "../control/control.h"

/**
 * @brief This function initializes the humidity sensor
 */
void humidity_sensor_init();

void read_humidity();