#include "gpio_driver.h"
#include "gpio_registers.h"

void gpio_set_direction(gpio_num_t gpio_pin, gpio_direction_t direction) {
    if (direction == GPIO_OUTPUT) {
        GPIO_ENABLE_REG = (1 << gpio_pin);
    }
}

void gpio_set_high(gpio_num_t gpio_pin) {
    GPIO_OUT_W1TS_REG = (1 << gpio_pin);
}

void gpio_set_low(gpio_num_t gpio_pin) {
    GPIO_OUT_W1TC_REG = (1 << gpio_pin);
}