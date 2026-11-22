#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "../config/private_config.h"
#include "string.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "../sensors/sensor_defines.h"

/**
 * @brief This function publishes data to the MQTT broker in Raspberry Pi
 * @param sensor_data struct that holds sensor type and sensor data
 */
void publish_data(sensorData_t *sensor_data);

/**
 * @brief This function initializes MQTT communication
 */
void mqtt_init();

#endif // PUBLISHER_H