#include "thermistor.h"
#include "../communication/gpio_config.h"

void read_temperature (void *pvParameters) {

    float temperature;

    onewire_addr_t addr_list[4];
    size_t found = 0;

    esp_err_t ret = ds18x20_scan_devices(FF_THERMISTOR_GPIO, addr_list, sizeof(addr_list), &found);

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

    ret = ds18x20_measure(FF_THERMISTOR_GPIO, addr_list[0], true);
    if (ret != ESP_OK) {
        printf("Scan failed: 0x%X (%s)\n", ret, esp_err_to_name(ret));
        return;
    }

    for( ;; )
        {
            ret = ds18b20_read_temperature(FF_THERMISTOR_GPIO, addr_list[0], &temperature);
            if (ret != ESP_OK) {
                printf("Error reading temp: %X\n", ret);
                return;
            }
            else {
                sensor_send_data(&temperature);
                vTaskDelay(10000 / portTICK_PERIOD_MS);
            }
        }
}