#include "encoder.h"

static const char *ENCODER_TAG = "encoder";
uint8_t encoder_addr = 0x36;

void encoder_init() {
    i2c_start();
    i2c_return_t ack1 = i2c_write_byte(encoder_addr << 1);
    ESP_LOGI(ENCODER_TAG, "ACK: %d", ack1);
    i2c_stop();
}

double read_angle() {
    i2c_start();
    i2c_return_t ack1 = i2c_write_byte(encoder_addr << 1);
    // ESP_LOGI(ENCODER_TAG, "ACK: %d", ack1);
    i2c_return_t ack2 = i2c_write_byte(0x0C);
    // ESP_LOGI(ENCODER_TAG, "ACK: %d", ack2);
    // i2c_return_t ack3 = i2c_write_byte(0x06);
    // ESP_LOGI(ENCODER_TAG, "ACK: %d", ack3);
    i2c_stop();

    vTaskDelay(pdMS_TO_TICKS(20));

    i2c_start();
    i2c_return_t ack3 = i2c_write_byte((encoder_addr << 1) | 1);
    // ESP_LOGI(ENCODER_TAG, "ACK: %d", ack3);
    uint8_t angle_high = i2c_read_byte(true);
    uint8_t angle_low = i2c_read_byte(false);
    i2c_stop();

    // ESP_LOGI(ENCODER_TAG, "ANGLE_HIGH: %d, ANGLE_LOW: %d", angle_high, angle_low);
    vTaskDelay(pdMS_TO_TICKS(100));

    return calculate_angle(angle_high, angle_low);
}

void encoder_debug_mode_on() {
    for (;;) {
            i2c_start();
    i2c_return_t ack1 = i2c_write_byte(encoder_addr << 1);
    // ESP_LOGI(ENCODER_TAG, "ACK: %d", ack1);
    i2c_return_t ack2 = i2c_write_byte(0x0C);
    // ESP_LOGI(ENCODER_TAG, "ACK: %d", ack2);
    // i2c_return_t ack3 = i2c_write_byte(0x06);
    // ESP_LOGI(ENCODER_TAG, "ACK: %d", ack3);
    i2c_stop();

    vTaskDelay(pdMS_TO_TICKS(20));

    i2c_start();
    i2c_return_t ack3 = i2c_write_byte((encoder_addr << 1) | 1);
    ESP_LOGI(ENCODER_TAG, "ACK: %d", ack3);
    uint8_t angle_high = i2c_read_byte(true);
    uint8_t angle_low = i2c_read_byte(false);
    i2c_stop();

    ESP_LOGI(ENCODER_TAG, "ANGLE_HIGH: %f", calculate_angle(angle_high, angle_low));
    vTaskDelay(pdMS_TO_TICKS(100));
    }
}