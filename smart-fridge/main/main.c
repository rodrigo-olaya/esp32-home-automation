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
#include "sensors/thermistor.h"
#include "control/control.h"
#include "communication/queues.h"

extern QueueHandle_t xSensorDataQueue;

void app_main(void)
{
    /* Initialize the queues */
    queues_init();

    /* Initialize controls - tasks and receiver code */
    controls_init();
}
