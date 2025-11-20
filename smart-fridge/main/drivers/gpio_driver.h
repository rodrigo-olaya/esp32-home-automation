#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
} gpio_direction_t;

typedef enum {
    GPIO_NUMBER_0 = 0,
    GPIO_NUMBER_1 = 1,
    GPIO_NUMBER_2 = 2,
    GPIO_NUMBER_3 = 3,
    GPIO_NUMBER_4 = 4,
    GPIO_NUMBER_5 = 5,
    GPIO_NUMBER_6 = 6, 
    GPIO_NUMBER_7 = 7,
    GPIO_NUMBER_8 = 8,
    GPIO_NUMBER_9 = 9,
    GPIO_NUMBER_10 = 10,
    GPIO_NUMBER_18 = 18
} gpio_number_t;

/**
 * @brief Set a GPIO pin as input or output
 * @param gpio_pin GPIO pin that needs to be set
 * @param direction GPIO_OUTPUT or GPIO_INPUT
 */
void gpio_set_dir(gpio_number_t gpio_pin, gpio_direction_t direction);

/**
 * @brief set a GPIO pin HIGH
 * @param gpio_pin GPIO pin that needs to be set
 */
void gpio_set_high(gpio_number_t gpio_pin);

/**
 * @brief set a GPIO pin LOW
 * @param gpio_pin GPIO pin that needs to be set
 */
void gpio_set_low(gpio_number_t gpio_pin);

#endif // GPIO_DRIVER_H