#include <sht3x.h>
#include "esp_log.h"
#include "../communication/gpio_config.h"
#include "sensor_defines.h"
#include "../control/control.h"
// #include "../communication/sensors.h"

// extern enum sensorData_t;

/**
 * @brief This function initializes the humidity sensor
 * @param test blah blah parameter.
 * @return 0 if blah blah passed.
 */
void humidity_sensor_init();

void read_humidity();