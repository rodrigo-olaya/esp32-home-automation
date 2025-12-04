#include "encoder.h"

static const char *ENCODER_TAG = "encoder";
uint8_t encoder_addr = 0x36;

void encoder_init() {
        i2c_start();
        i2c_return_t ack1 = i2c_write_byte(encoder_addr << 1);
        ESP_LOGI(ENCODER_TAG, "ACK: %d", ack1);
        i2c_stop();
    }