#include <inttypes.h>

#include "../drivers/gpio_driver.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "mqtt_client.h"

#include "handlers/calculate_steps.h"

#ifndef NEMA17_H
#define NEMA17_H

typedef enum {
    CLOCKWISE = 0,
    COUNTERCLOCKWISE = 1
} motor_direction_t;

void configure_motor(esp_mqtt_event_handle_t event);

void set_step_resolution();

void set_motor_speed(uint8_t step_time_in_msec);

void set_direction(motor_direction_t direction);

void step();

void move_to_angle(float target_angle);

#endif