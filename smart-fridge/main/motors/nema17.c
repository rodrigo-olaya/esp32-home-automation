#include "nema17.h"

static uint8_t step_delay_in_msec = 50;
static float step_resolution = 1.8;

void set_step_resolution() {}

void set_motor_speed(uint8_t target_delay_in_msec){
    step_delay_in_msec = target_delay_in_msec;
}

void step(gpio_number_t motor_step_gpio) {
    gpio_set_high(motor_step_gpio);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
    gpio_set_low(motor_step_gpio);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
}

void move_to_angle(float target_angle) {}

void drive_motor(){
    gpio_number_t motor_step_gpio = GPIO_NUMBER_5;
    gpio_number_t motor_dir_gpio = GPIO_NUMBER_18;

    gpio_set_dir(motor_step_gpio, GPIO_OUTPUT);  // STEP
    gpio_set_dir(motor_dir_gpio, GPIO_OUTPUT); // DIR

    gpio_set_low(motor_dir_gpio);  // Set direction

    set_motor_speed(25);

    while(1){
        step(motor_step_gpio);
    }    
}