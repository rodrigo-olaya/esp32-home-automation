#include "humidity_sensor.h"

static const char *HUMIDITY_TAG = "SHT3x";
uint8_t sht3x_addr = 0x44;

void humidity_sensor_initialize() {
    // uint8_t sht3x_addr = 0x44;

    i2c_init();
    // i2c_start();
    // i2c_return_t ack = i2c_write_byte(sht3x_addr << 1);
    // i2c_stop();
    // ESP_LOGI(HUMIDITY_TAG, "ACK: %d", ack);
}

void read_humidity_new() {

    i2c_start();
    i2c_return_t ack1 = i2c_write_byte(0x44 << 1);
    ESP_LOGI(HUMIDITY_TAG, "ACK: %d", ack1);
    i2c_return_t ack2 = i2c_write_byte(0x2C);
    ESP_LOGI(HUMIDITY_TAG, "ACK: %d", ack2);
    i2c_return_t ack3 = i2c_write_byte(0x06);
    ESP_LOGI(HUMIDITY_TAG, "ACK: %d", ack3);
    i2c_stop();

    vTaskDelay(pdMS_TO_TICKS(20));

    i2c_start();
    i2c_write_byte((0x44 << 1) | 1);
    uint8_t temp_msb = i2c_read_byte(true);
    uint8_t temp_lsb = i2c_read_byte(true);
    uint8_t temp_crc = i2c_read_byte(true);
    uint8_t hum_msb = i2c_read_byte(true);
    uint8_t hum_lsb = i2c_read_byte(true);
    uint8_t hum_crc = i2c_read_byte(false);
    i2c_stop();
    ESP_LOGI(HUMIDITY_TAG, "HUMIDITY MSB: %d, HUMIDITY LSB: %d", hum_msb, hum_lsb);
}