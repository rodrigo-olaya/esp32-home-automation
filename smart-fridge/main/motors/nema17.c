#include "nema17.h"

static uint8_t step_delay_in_msec = 50;

void set_step_resolution() {}

void set_motor_speed(uint8_t target_delay_in_msec){
    step_delay_in_msec = target_delay_in_msec;
}

void step() {
    gpio_set_high(GPIO_NUMBER_5);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
    gpio_set_low(GPIO_NUMBER_5);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
}

void move_to_angle(float target_angle) {}

void drive_motor(){
    gpio_set_dir(GPIO_NUMBER_5, GPIO_OUTPUT);  // STEP
    gpio_set_dir(GPIO_NUMBER_18, GPIO_OUTPUT); // DIR

    gpio_set_low(GPIO_NUMBER_18);  // Set direction

    set_motor_speed(25);

    while(1){
        step();
    }    
}