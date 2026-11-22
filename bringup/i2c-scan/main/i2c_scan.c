/*
 * Bring-up: what is on the I2C bus?
 * First checks that SDA and SCL idle high (a low line means a short or a missing pull-up),
 * then probes every address once every two seconds.
 * Pins match the carrier board: SDA = GPIO21, SCL = GPIO22 (ports J5/J6/J7).
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"

#define I2C_SDA GPIO_NUM_21
#define I2C_SCL GPIO_NUM_22

static const char *device_name(uint8_t addr)
{
    switch (addr) {
        case 0x36: return "AS5600 magnetic encoder";
        case 0x3C:
        case 0x3D: return "SSD1306 OLED";
        case 0x44:
        case 0x45: return "SHT3x temperature/humidity";
        default:   return "unknown";
    }
}

/* Reads a pin with only its internal pull-up on. Anything but high is a problem. */
static int idle_level(gpio_num_t pin)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    vTaskDelay(pdMS_TO_TICKS(10));
    return gpio_get_level(pin);
}

void app_main(void)
{
    printf("\n=== i2c-scan (SDA GPIO%d, SCL GPIO%d) ===\n", I2C_SDA, I2C_SCL);

    printf("SDA idle: %s\n", idle_level(I2C_SDA) ? "high (ok)" : "LOW: stuck or shorted to GND");
    printf("SCL idle: %s\n", idle_level(I2C_SCL) ? "high (ok)" : "LOW: stuck or shorted to GND");

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        /* Breadboard has no R3/R4 yet. Harmless when the real 4.7k pull-ups are there. */
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t bus;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus));

    for (;;) {
        int found = 0;
        for (uint8_t addr = 0x08; addr < 0x78; addr++) {
            if (i2c_master_probe(bus, addr, 50) == ESP_OK) {
                printf("  0x%02X  %s\n", addr, device_name(addr));
                found++;
            }
        }
        printf("found %d device(s)\n\n", found);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
