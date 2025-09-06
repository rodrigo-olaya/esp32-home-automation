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

// /* This function was borrowed from FreeRTOS book as a template*/
// static void vSenderTask( void *pvParameters )
// {
//     int32_t lValueToSend;
//     BaseType_t xStatus;
//     /* Two instances of this task are created so the value that is sent to
//     the queue is passed in via the task parameter - this way each instance
//     can use a different value. The queue was created to hold values of type
//     int32_t, so cast the parameter to the required type. */
//     lValueToSend = ( int32_t ) pvParameters;
//     /* As per most tasks, this task is implemented within an infinite loop. */
//     for( ;; )
//     {
//         /* Send the value to the queue.
//         The first parameter is the queue to which data is being sent. The
//         queue was created before the scheduler was started, so before this
//         task started to execute.
//         The second parameter is the address of the data to be sent, in this
//         case the address of lValueToSend.
//         The third parameter is the Block time – the time the task should be
//         kept in the Blocked state to wait for space to become available on
//         the queue should the queue already be full. In this case a block
//         time is not specified because the queue should never contain more
//         than one item, and therefore never be full. */
//         xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );
//         if( xStatus != pdPASS )
//         {
//             /* The send operation could not complete because the queue was full-
//             this must be an error as the queue should never contain more than
//             one item! */
//             // vPrintString( "Could not send to the queue.\r\n" );
//             printf("Could not send data to the queue.\n");
//         }
//         // vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }

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
        // vPrintString( "Queue should have been empty!\r\n" );
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
        // vPrintStringAndNumber( "Received = ", lReceivedValue );
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
        /* Create two instances of the task that will send to the queue. The
        task parameter is used to pass the value that the task will write
        to the queue, so one task will continuously write 100 to the queue
        while the other task will continuously write 200 to the queue. Both
        tasks are created at priority 1. */
        xTaskCreate( read_temperature, "Sender1", 4000, NULL, 1, NULL );
        // xTaskCreate( vSenderTask, "Sender2", 4000, ( void * ) 200, 1, NULL );
        /* Create the task that will read from the queue. The task is created
        with priority 2, so above the priority of the sender tasks. */
        xTaskCreate( vReceiverTask, "Receiver", 2048, NULL, 2, NULL );
        /* Start the scheduler so the created tasks start executing. */
        // vTaskStartScheduler();
    } else {
        // queue could not be created
    }

    

    // vTaskDelay(500 / portTICK_PERIOD_MS);

    // fflush(stdout);
    // esp_restart();
}
