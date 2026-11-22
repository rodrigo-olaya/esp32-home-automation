#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../MQTT/publisher.h"
#include "../sensors/sensor_defines.h"
#include "../sensors/sht31.h"
#include "../discovery/discovery.h"
#include "esp_timer.h"

extern QueueHandle_t xSensorDataQueue;

/**
 * @brief This function reads the temperature from the DS18B20 and calls the sensor control function with the function.
 * @param sensor_data
 */
void sensor_send_data(sensorData_t *sensor_data);

void vReceiverTask( void *pvParameters );

void controls_init();

/**
 * @brief discovery_actions_t callbacks: create or delete the task for one sensor.
 * @param sensor SENSOR_SHT31 or SENSOR_DS18B20
 */
void start(sensor_flag_t sensor);
void stop(sensor_flag_t sensor);

/**
 * @brief Called by a sensor task after a valid reading. Records the time for the discovery task.
 * @param sensor SENSOR_SHT31 or SENSOR_DS18B20
 */
void mark_sensor_seen(sensor_flag_t sensor);

void discovery_task();

#endif