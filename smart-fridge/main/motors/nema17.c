#include "nema17.h"

void set_step_resolution() {}

void set_motor_speed(uint8_t step_time_in_msec){}

void drive_motor(){
    gpio_set_dir(GPIO_NUMBER_5, GPIO_OUTPUT);  // STEP
    gpio_set_dir(GPIO_NUMBER_18, GPIO_OUTPUT); // DIR

    gpio_set_low(GPIO_NUMBER_18);  // Set direction

    while(1) {
        gpio_set_high(GPIO_NUMBER_5);
        vTaskDelay(25 / portTICK_PERIOD_MS);
        gpio_set_low(GPIO_NUMBER_5);
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}