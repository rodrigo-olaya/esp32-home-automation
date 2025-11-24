#include "humidity_sensor.h"

static const char *HUMIDITY_TAG = "SHT3x";

void humidity_sensor_initialize() {
    uint8_t sht3x_addr = 0x44;

    i2c_init();
    i2c_start();
    i2c_return_t ack = i2c_write_byte(sht3x_addr << 1);
    i2c_stop();
    ESP_LOGI(HUMIDITY_TAG, "ACK: %d", ack);
}