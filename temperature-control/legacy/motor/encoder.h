#ifndef ENCODER_H
#define ENCODER_H

#include <inttypes.h>
#include "../drivers/i2c_driver.h"
#include "esp_log.h"
#include "handlers/calculate_angle.h"

void encoder_init();

double read_angle();

void encoder_debug_mode_on();

#endif