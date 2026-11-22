#ifndef TOPIC_H
#define TOPIC_H

#include <stdbool.h>
#include <stddef.h>
#include "../sensors/sensor_defines.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Builds the MQTT topic for one reading: "home/esp32/<location>/temp" for TEMPERATURE and
 * "home/esp32/<location>/humidity" for HUMIDITY. Pure string work, so it runs on the host tests.
 * @param out      buffer that receives the NUL-terminated topic
 * @param out_size size of out in bytes, including room for the NUL
 * @param location where the sensor is, for example "bedroom"
 * @param type     TEMPERATURE or HUMIDITY
 * @return true if the whole topic fit in out, false if it did not. After a false the contents of
 * out are unspecified (it may hold a cut-off topic), so callers must check the result and not use out.
 */
bool build_topic(char *out, size_t out_size, const char *location, sensorType_t type);

#endif // TOPIC_H
