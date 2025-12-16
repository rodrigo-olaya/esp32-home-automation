#include "onewire.h"

static gpio_number_t onewire_pin;

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
    if (bit == 1){
        gpio_set_dir(onewire_pin, GPIO_OUTPUT);
        gpio_set_low(onewire_pin);
        ets_delay_us(10);
        gpio_set_dir(onewire_pin, GPIO_INPUT);
        ets_delay_us(50);
    }
    else if (bit == 0) {
        gpio_set_dir(onewire_pin, GPIO_OUTPUT);
        gpio_set_low(onewire_pin);
        ets_delay_us(60);
        gpio_set_dir(onewire_pin, GPIO_INPUT);
    }
}

int onewire_read_bit(void) {
    gpio_set_dir(onewire_pin, GPIO_OUTPUT);
    gpio_set_low(onewire_pin);
    ets_delay_us(1);
    gpio_set_dir(onewire_pin, GPIO_INPUT);

    ets_delay_us(14);

    int data = gpio_read(onewire_pin);

    ets_delay_us(45);

    return data;
}

void onewire_write_byte(uint8_t byte) {

}

uint8_t onewire_read_byte(void) {

}