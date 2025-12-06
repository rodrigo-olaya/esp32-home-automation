#ifndef NEMA17_H
#define NEMA17_H

#include <inttypes.h>

#include "../drivers/gpio_driver.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "encoder.h"
#include "handlers/calculate_steps.h"
#include "handlers/calculate_angle.h"

typedef enum {
    CLOCKWISE = 0,
    COUNTERCLOCKWISE = 1
} motor_direction_t;

/**
 * @brief Configures GPIO direction pin according to command
 * @param event MQTT event that carries what the motor needs to do
 */
void configure_motor(esp_mqtt_event_handle_t event);

/**
 * @brief 
 * @param 
 */
void set_step_resolution();

/**
 * @brief 
 * @param 
 */
void set_motor_speed(uint8_t step_time_in_msec);

/**
 * @brief 
 * @param 
 */
void set_direction(motor_direction_t direction);

/**
 * @brief 
 * @param 
 */
void step();

/**
 * @brief 
 * @param 
 */
void move_to_angle(float target_angle);

#endif