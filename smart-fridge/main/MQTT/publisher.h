#include "../config/private_config.h"
#include "string.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "../sensors/sensor_defines.h"

#define TEMP_TOPIC "home/esp32/livingroom/temp"
#define HUMIDITY_TOPIC "home/esp32/livingroom/humidity"

void publish_data(sensorData_t sensor_data);

void mqtt_init();
