#include "sht31.h"
#include "../control/control.h"

static const char *SHT31_TAG = "SHT31";
uint8_t sht31_addr = 0x44;

void sht31_init() {
    i2c_init();
}

void sht31_task() {
    for (;;) {
        i2c_lock();
        i2c_start();
        i2c_return_t ack1 = i2c_write_byte(sht31_addr << 1);
        ESP_LOGI(SHT31_TAG, "ACK: %d", ack1);
        i2c_return_t ack2 = i2c_write_byte(0x2C);
        ESP_LOGI(SHT31_TAG, "ACK: %d", ack2);
        i2c_return_t ack3 = i2c_write_byte(0x06);
        ESP_LOGI(SHT31_TAG, "ACK: %d", ack3);
        i2c_stop();

        vTaskDelay(pdMS_TO_TICKS(20));

        i2c_start();
        i2c_write_byte((sht31_addr << 1) | 1);
        uint8_t temp_msb = i2c_read_byte(true);
        uint8_t temp_lsb = i2c_read_byte(true);
        uint8_t temp_crc = i2c_read_byte(true);
        uint8_t hum_msb = i2c_read_byte(true);
        uint8_t hum_lsb = i2c_read_byte(true);
        uint8_t hum_crc = i2c_read_byte(false);
        i2c_stop();
        ESP_LOGI(SHT31_TAG, "HUMIDITY MSB: %d, HUMIDITY LSB: %d", hum_msb, hum_lsb);

        double humidity = bytes_to_humidity(hum_msb, hum_lsb);

        sensorData_t sensor_data;
        sensor_data.type = HUMIDITY;
        sensor_data.data = humidity;

        sensor_send_data(&sensor_data);
        ESP_LOGI(SHT31_TAG, "Humidity: %f", sensor_data.data);

        i2c_unlock();

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}