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
    xSensorDataQueue = xQueueCreate( 5, sizeof( float ) );

    if( xSensorDataQueue != NULL ) {
        /* Create the task that will read the thermistor data and send it to the receiver */
        xTaskCreate( read_temperature, "Sender1", 4000, NULL, 1, NULL );
        /* Create the task that will read from the queue. The task is created
        with priority 2, so above the priority of the sender tasks. */
        xTaskCreate( vReceiverTask, "Receiver", 2048, NULL, 2, NULL );
    } else {
        // queue could not be created
    }
}
