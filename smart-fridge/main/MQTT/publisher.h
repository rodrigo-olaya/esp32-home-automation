#include "../config/private_config.h"
#include "string.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "../sensors/sensor_defines.h"

#define TEMP_TOPIC "home/esp32/livingroom/temp"
#define HUMIDITY_TOPIC "home/esp32/livingroom/humidity"
#define HEATER_TOPIC "home/esp32/controls/heater"

/**
 * @brief This function publishes data to the MQTT broker in Raspberry Pi
 * @param sensor_data struct that holds sensor type and sensor data
 */
void publish_data(sensorData_t *sensor_data);

void receive_data();

/**
 * @brief This function initializes MQTT communication
 */
void mqtt_init();
