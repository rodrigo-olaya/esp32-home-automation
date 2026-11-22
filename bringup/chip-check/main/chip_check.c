/*
 * Bring-up step 0: is the DevKit alive?
 * Prints what the chip says about itself, then blinks the status LED (GPIO2) once a second.
 * GPIO2 is the DevKit's own LED and also drives D5 on the carrier board.
 */
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_mac.h"
#include "esp_system.h"

#define STATUS_LED GPIO_NUM_2

static const char *reset_reason_name(esp_reset_reason_t reason)
{
    switch (reason) {
        case ESP_RST_POWERON:   return "power on";
        case ESP_RST_SW:        return "software reset";
        case ESP_RST_PANIC:     return "panic";
        case ESP_RST_INT_WDT:   return "interrupt watchdog";
        case ESP_RST_TASK_WDT:  return "task watchdog";
        case ESP_RST_WDT:       return "other watchdog";
        case ESP_RST_DEEPSLEEP: return "deep sleep wake";
        case ESP_RST_BROWNOUT:  return "BROWNOUT (supply sagged)";
        default:                return "other";
    }
}

void app_main(void)
{
    esp_chip_info_t chip;
    esp_chip_info(&chip);

    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, &flash_size);

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    printf("\n=== chip-check ===\n");
    printf("model:        ESP32 rev v%d.%d, %d core(s)\n", chip.revision / 100, chip.revision % 100, chip.cores);
    printf("features:     %s%s%s\n",
           (chip.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi " : "",
           (chip.features & CHIP_FEATURE_BT) ? "BT " : "",
           (chip.features & CHIP_FEATURE_BLE) ? "BLE" : "");
    printf("flash:        %" PRIu32 " MB\n", flash_size / (1024 * 1024));
    printf("MAC:          %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf("free heap:    %" PRIu32 " bytes\n", esp_get_free_heap_size());
    printf("reset reason: %s\n", reset_reason_name(esp_reset_reason()));

    gpio_reset_pin(STATUS_LED);
    gpio_set_direction(STATUS_LED, GPIO_MODE_OUTPUT);

    for (int count = 0;; count++) {
        gpio_set_level(STATUS_LED, count & 1);
        printf("alive %d  LED %s\n", count, (count & 1) ? "on" : "off");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
