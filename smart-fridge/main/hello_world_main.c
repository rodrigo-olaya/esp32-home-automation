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
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "onewire.h"
#include "ds18x20.h"
#include <inttypes.h>
#include <portmacro.h>
#include "esp_mac.h"

#define GPIO_NUM 4

void app_main(void)
{
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

    ret = ds18b20_read_temperature(GPIO_NUM, addr_list[0], &temperature);
    if (ret != ESP_OK) {
        printf("Error reading temp: %X\n", ret);
        return;
    }
    else {
        printf("Temperature: %.2f\n", temperature);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);

    fflush(stdout);
    esp_restart();
}
