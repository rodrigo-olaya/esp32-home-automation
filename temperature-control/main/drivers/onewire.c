#include "onewire.h"

static gpio_number_t onewire_pin;

static portMUX_TYPE onewire_mux = portMUX_INITIALIZER_UNLOCKED;

void onewire_init(int gpio_pin) {
    onewire_pin = gpio_pin;
    gpio_set_dir(onewire_pin, GPIO_INPUT);
}

int onewire_reset(void) {
    gpio_set_dir(onewire_pin, GPIO_OUTPUT);
    gpio_set_low(onewire_pin);
    ets_delay_us(480);

    gpio_set_dir(onewire_pin, GPIO_INPUT);
    ets_delay_us(70);

    int present = (gpio_read(onewire_pin) == LOW) ? 1 : 0;

    ets_delay_us(410);

    return present;
}

void onewire_write_bit(int bit) {
    portENTER_CRITICAL(&onewire_mux);
    if (bit == 1){
        gpio_set_dir(onewire_pin, GPIO_OUTPUT);
        gpio_set_low(onewire_pin);
        ets_delay_us(6);
        gpio_set_dir(onewire_pin, GPIO_INPUT);
        ets_delay_us(64);
    }
    else if (bit == 0) {
        gpio_set_dir(onewire_pin, GPIO_OUTPUT);
        gpio_set_low(onewire_pin);
        ets_delay_us(60);
        gpio_set_dir(onewire_pin, GPIO_INPUT);
        ets_delay_us(10);
    }
    portEXIT_CRITICAL(&onewire_mux);
}

int onewire_read_bit(void) {
    portENTER_CRITICAL(&onewire_mux);
    gpio_set_dir(onewire_pin, GPIO_OUTPUT);
    gpio_set_low(onewire_pin);
    ets_delay_us(3);
    gpio_set_dir(onewire_pin, GPIO_INPUT);

    ets_delay_us(12);

    int data = gpio_read(onewire_pin);

    ets_delay_us(50);
    portEXIT_CRITICAL(&onewire_mux);

    return data;
}

void onewire_write_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        onewire_write_bit( (byte >> i) & 1);
    }
}

uint8_t onewire_read_byte(void) {
    uint8_t data = 0;

    for (int i = 0; i < 8; i++) {
        data |= onewire_read_bit() << i;
    }

    return data;
}