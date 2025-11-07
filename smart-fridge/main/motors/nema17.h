#include <inttypes.h>

#include "../drivers/gpio_driver.h"
#include "freertos/FreeRTOS.h"

void set_step_resolution();

void set_motor_speed(uint8_t step_time_in_msec);

void drive_motor();