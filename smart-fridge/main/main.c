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
#include <cJSON.h>

extern QueueHandle_t xSensorDataQueue;

typedef struct {
    char pi4_ip[14];
    uint16_t port;
} MQTT_config;

void app_main(void)
{
    // Open file in read mode
    FILE *private_data= fopen("JSON/private.json", "r");  

    if (private_data == NULL) {
        printf("Error!\n");
        return 1;
    }

    /* Get the length of the file*/
    fseek(private_data, 0, SEEK_END);
    long len = ftell(private_data);
    fseek(private_data, 0, SEEK_SET);

    /* Create the data structure needed for the parsing API*/
    char* data = (char*)malloc(len + 1);
    fread(data, 1, len, private_data);
    data[len] = '\0';

    printf("Successfull\n");

    // Close the file
    fclose(private_data);  

    cJSON *IP_addresses = cJSON_Parse(data);

    free(data);

    cJSON_Delete(IP_addresses);

    for(;;);

    /* Initialize the queues */
    queues_init();

    /* Initialize controls - tasks and receiver code */
    controls_init();
}
