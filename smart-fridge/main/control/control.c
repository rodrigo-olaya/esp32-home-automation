#include "control.h"

void vReceiverTask( void *pvParameters )
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
        if( uxQueueMessagesWaiting( xSensorDataQueue ) != 0 )
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
        xStatus = xQueueReceive( xSensorDataQueue, &lReceivedValue, xTicksToWait );
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