#include "gpio_driver.h"
#include "gpio_registers.h"

void gpio_set_dir(gpio_number_t gpio_pin, gpio_direction_t direction) {
    if (direction == GPIO_OUTPUT) {
        GPIO_ENABLE_REG |= (1 << gpio_pin);
    }
    else if (direction == GPIO_INPUT) {
        GPIO_ENABLE_W1TC_REG = (1 << gpio_pin);
    }
}

void gpio_set_high(gpio_number_t gpio_pin) {
    GPIO_OUT_W1TS_REG = (1 << gpio_pin);
}

void gpio_set_low(gpio_number_t gpio_pin) {
    GPIO_OUT_W1TC_REG = (1 << gpio_pin);
}

gpio_state_t gpio_read(gpio_number_t gpio_pin) {
    return GPIO_IN_REG & (1 << gpio_pin) ? HIGH : LOW;
}