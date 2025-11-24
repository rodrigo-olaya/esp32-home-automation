/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include <inttypes.h>
#include <portmacro.h>
#include "esp_mac.h"
// #include "sensors/thermistor.h"
#include "control/control.h"
#include "communication/queues.h"
#include "WiFi/WiFi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sensors/SHT_3x.h"
#include "drivers/gpio_driver.h"
#include "motors/nema17.h"
#include "sensors/humidity_sensor.h"

extern QueueHandle_t xSensorDataQueue;

void app_main(void)
{
    // /* Initialize WiFi */
    // WiFi_init();

    // /* Initialize the queues */
    // queues_init();

    // /* Initialize MQTT client once */
    // mqtt_init();

    // /* Initialize controls - tasks and receiver code */
    // controls_init();

    humidity_sensor_initialize();
}
