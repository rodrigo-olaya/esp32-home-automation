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
#include "thermistor.h"

QueueHandle_t xQueue;

static void vReceiverTask( void *pvParameters )
{
/* Declare the variable that will hold the values received from the
queue. */
float lReceivedValue;
BaseType_t xStatus;
const TickType_t xTicksToWait = pdMS_TO_TICKS( 100000 );
/* This task is also defined within an infinite loop. */
for( ;; )
{
    /* This call should always find the queue empty because this task will
    immediately remove any data that is written to the queue. */
    if( uxQueueMessagesWaiting( xQueue ) != 0 )
    {
        printf("Queue should have been empty!\n");
    }
    /* Receive data from the queue.
    The first parameter is the queue from which data is to be received.
    The queue is created before the scheduler is started, and therefore
    before this task runs for the first time.
    The second parameter is the buffer into which the received data will
    be placed. In this case the buffer is simply the address of a
    variable that has the required size to hold the received data.
    The last parameter is the block time – the maximum amount of time
    that the task will remain in the Blocked state to wait for data to
    be available should the queue already be empty. */
    xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
    if( xStatus == pdPASS )
    {
        /* Data was successfully received from the queue, print out the
        received value. */
        printf("Number: %.2f\n", lReceivedValue);
    }
    else
    {
        /* Data was not received from the queue even after waiting for
        100ms. This must be an error as the sending tasks are free
        running and will be continuously writing to the queue. */
        printf("Could not receive from the queue.\n");
    }
}
}

void app_main(void)
{
    xQueue = xQueueCreate( 5, sizeof( float ) );

    if( xQueue != NULL ) {
        /* Create the task that will read the thermistor data and send it to the receiver */
        xTaskCreate( read_temperature, "Sender1", 4000, NULL, 1, NULL );
        /* Create the task that will read from the queue. The task is created
        with priority 2, so above the priority of the sender tasks. */
        xTaskCreate( vReceiverTask, "Receiver", 2048, NULL, 2, NULL );
    } else {
        // queue could not be created
    }
}
