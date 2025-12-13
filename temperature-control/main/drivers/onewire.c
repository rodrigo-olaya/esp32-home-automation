#include "onewire.h"

static gpio_number_t onewire_pin;

void onewire_init(int gpio_pin) {
    onewire_pin = gpio_pin;
    gpio_set_dir(onewire_pin, GPIO_INPUT);
}

