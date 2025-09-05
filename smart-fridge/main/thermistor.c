#include "thermistor.h"

void read_temperature (void *pvParameters) {

    float temperature;

    onewire_addr_t addr_list[4];
    size_t found = 0;

    esp_err_t ret = ds18x20_scan_devices(GPIO_NUM, addr_list, sizeof(addr_list), &found);

    if (ret != ESP_OK) {
        printf("Error finding devices: %X\n", ret);
        return;
    }
    else {
        uint8_t *addr_bytes = (uint8_t*)&addr_list[0];
        printf("Address: ");
        for (int i = 0; i < 8; i++) {
            printf("%02X", addr_bytes[i]);
        }
        printf("\n");
    }

    ret = ds18x20_measure(GPIO_NUM, addr_list[0], true);
    if (ret != ESP_OK) {
        printf("Scan failed: 0x%X (%s)\n", ret, esp_err_to_name(ret));
        return;
    }

    // ret = ds18b20_read_temperature(GPIO_NUM, addr_list[0], &temperature);
    // if (ret != ESP_OK) {
    //     printf("Error reading temp: %X\n", ret);
    //     return;
    // }
    // else {
    //     printf("Temperature: %.2f\n", temperature);
    // }

    BaseType_t xStatus;

    for( ;; )
    {
        /* Send the value to the queue.
        The first parameter is the queue to which data is being sent. The
        queue was created before the scheduler was started, so before this
        task started to execute.
        The second parameter is the address of the data to be sent, in this
        case the address of lValueToSend.
        The third parameter is the Block time – the time the task should be
        kept in the Blocked state to wait for space to become available on
        the queue should the queue already be full. In this case a block
        time is not specified because the queue should never contain more
        than one item, and therefore never be full. */

        ret = ds18b20_read_temperature(GPIO_NUM, addr_list[0], &temperature);
        if (ret != ESP_OK) {
            printf("Error reading temp: %X\n", ret);
            return;
        }
        else {
            // printf("Temperature: %.2f\n", temperature);
            xStatus = xQueueSendToBack( xQueue, &temperature, 0 );
            if( xStatus != pdPASS )
            {
                /* The send operation could not complete because the queue was full-
                this must be an error as the queue should never contain more than
                one item! */
                // vPrintString( "Could not send to the queue.\r\n" );
                printf("Could not send data to the queue.\n");
            }
        // vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}