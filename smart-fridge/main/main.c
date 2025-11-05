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
    printf("Stabilizing\n");
    // vTaskDelay(100 / portTICK_PERIOD_MS);

    gpio_number_t led_pin = GPIO_NUMBER_2;
    gpio_set_low(led_pin);
    printf("Stabilizing pt2\n");
    // vTaskDelay(3000 / portTICK_PERIOD_MS);
    gpio_set_dir(led_pin, GPIO_OUTPUT);
    printf("Stabilizing pt3\n");
    // vTaskDelay(3000 / portTICK_PERIOD_MS);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    while(1) {
        gpio_set_high(led_pin);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_low(led_pin);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
