#include <inttypes.h>

#include "../drivers/gpio_driver.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

#include "handlers/calculate_steps.h"

void set_step_resolution();

void set_motor_speed(uint8_t step_time_in_msec);

void set_direction(uint8_t direction);

void step();

void move_to_angle(float target_angle);