#include "thermistor.h"

static const char* THERMISTOR_TAG = "thermistor";

// void read_temperature (void *pvParameters) {

//     float temperature;

//     onewire_addr_t addr_list[4];
//     size_t found = 0;

//     esp_err_t ret = ds18x20_scan_devices(FF_THERMISTOR_GPIO, addr_list, sizeof(addr_list), &found);

//     if (ret != ESP_OK) {
//         ESP_LOGE(THERMISTOR_TAG, "Error finding devices: %X", ret);
//         return;
//     }
//     else {
//         uint8_t *addr_bytes = (uint8_t*)&addr_list[0];
//         printf("Address: ");
//         for (int i = 0; i < 8; i++) {
//             printf("%02X", addr_bytes[i]);
//         }
//         printf("\n");
//     }

//     for( ;; )
//         {
//             /* This function is needed before each thermistor read */
//             ret = ds18x20_measure(FF_THERMISTOR_GPIO, addr_list[0], true);
//             if (ret != ESP_OK) {
//                 ESP_LOGE(THERMISTOR_TAG, "Scan failed: 0x%X (%s)", ret, esp_err_to_name(ret));
//                 return;
//             }
//             /* Get one reading from thermistor */
//             ret = ds18b20_read_temperature(FF_THERMISTOR_GPIO, addr_list[0], &temperature);
//             if (ret != ESP_OK) {
//                 ESP_LOGE(THERMISTOR_TAG, "Error reading temp: %X", ret);
//                 return;
//             }
//             else {
//                 ESP_LOGI(THERMISTOR_TAG, "Sending data to queue");
//                 ESP_LOGI(THERMISTOR_TAG, "Data sent to queue: %f", temperature);

//                 sensorData_t sensor_data;
//                 sensor_data.type = TEMPERATURE;
//                 sensor_data.data = round(convert_to_farenheit(temperature));

//                 sensor_send_data(&sensor_data);
//                 vTaskDelay(pdMS_TO_TICKS(10000));
//             }
//         }
// }

void read_temperature (void *pvParameters) {
    onewire_init(GPIO_NUMBER_4);

    // int present = onewire_reset();
    // ESP_LOGI("1-WIRE", "Device present: %d", present);

    // onewire_write_byte(0xCC);
    // onewire_write_byte(0x44);
    // vTaskDelay(pdMS_TO_TICKS(750));

    for (;;) {
        uint8_t scratchpad[9];
        int present = onewire_reset();
        ESP_LOGI("1-WIRE", "Device present: %d", present);

        onewire_write_byte(0xCC);
        onewire_write_byte(0x44);
        vTaskDelay(pdMS_TO_TICKS(750));

        onewire_reset();

        onewire_write_byte(0xCC);
        onewire_write_byte(0xBE);

        for (int i = 0; i < 9; i++) {
            scratchpad[i] = onewire_read_byte();
        }

        ESP_LOGI(THERMISTOR_TAG, "Scratchpad: %02X %02X %02X %02X %02X %02X %02X %02X %02X",
            scratchpad[0], scratchpad[1], scratchpad[2], scratchpad[3], scratchpad[4],
            scratchpad[5], scratchpad[6], scratchpad[7], scratchpad[8]);
    
        // uint8_t temperature_lsb = onewire_read_byte();
        // uint8_t temperature_msb = onewire_read_byte();

        float temperature = raw_onewire_to_temp(scratchpad[1], scratchpad[0]);
        ESP_LOGI(THERMISTOR_TAG, "Data sent to queue: %f", temperature);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}