/*
 * Bring-up: does the SHT31 give sane readings?
 * Single-shot, high repeatability, no clock stretching. Every word is CRC-checked.
 * Pins match the carrier board: SDA = GPIO21, SCL = GPIO22.
 */
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"

#define I2C_SDA GPIO_NUM_21
#define I2C_SCL GPIO_NUM_22
#define SHT31_ADDR 0x44    /* 0x45 if the ADDR pin is pulled high */

static const uint8_t CMD_MEASURE[2] = {0x24, 0x00};

/* CRC-8, polynomial 0x31, init 0xFF (SHT3x datasheet). */
static uint8_t crc8(const uint8_t *data, int len)
{
    uint8_t crc = 0xFF;
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int bit = 0; bit < 8; bit++) {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

void app_main(void)
{
    printf("\n=== sht31-read (addr 0x%02X) ===\n", SHT31_ADDR);

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t bus;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SHT31_ADDR,
        .scl_speed_hz = 100000,
    };
    i2c_master_dev_handle_t sensor;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus, &dev_config, &sensor));

    for (;;) {
        uint8_t raw[6];
        esp_err_t err = i2c_master_transmit(sensor, CMD_MEASURE, sizeof(CMD_MEASURE), 100);
        if (err == ESP_OK) {
            vTaskDelay(pdMS_TO_TICKS(20));    /* high repeatability takes up to 15 ms */
            err = i2c_master_receive(sensor, raw, sizeof(raw), 100);
        }

        if (err != ESP_OK) {
            printf("I2C error: %s (check wiring, address, pull-ups)\n", esp_err_to_name(err));
        }
        else if (crc8(&raw[0], 2) != raw[2] || crc8(&raw[3], 2) != raw[5]) {
            printf("CRC mismatch: noisy bus or a bad sensor\n");
        }
        else {
            uint16_t raw_temp = (raw[0] << 8) | raw[1];
            uint16_t raw_hum = (raw[3] << 8) | raw[4];
            float celsius = -45.0f + 175.0f * raw_temp / 65535.0f;
            float humidity = 100.0f * raw_hum / 65535.0f;
            printf("T = %.2f C (%.2f F)   RH = %.1f %%\n", celsius, celsius * 9.0f / 5.0f + 32.0f, humidity);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
