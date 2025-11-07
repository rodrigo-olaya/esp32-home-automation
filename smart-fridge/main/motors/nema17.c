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

    // gpio_number_t led_pin = GPIO_NUMBER_2;
    // gpio_set_low(led_pin);
    // gpio_set_dir(led_pin, GPIO_OUTPUT);
    // vTaskDelay(500 / portTICK_PERIOD_MS);

    // while(1) {
    //     gpio_set_high(led_pin);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //     gpio_set_low(led_pin);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

    // gpio_number_t led_pin = GPIO_NUMBER_2;
    // gpio_number_t led_pin = GPIO_NUMBER_2;



    // gpio_set_dir(GPIO_NUMBER_5, GPIO_OUTPUT);  // STEP
    // gpio_set_dir(GPIO_NUMBER_18, GPIO_OUTPUT); // DIR

    // gpio_set_low(GPIO_NUMBER_18);  // Set direction

    // while(1) {
    //     gpio_set_high(GPIO_NUMBER_5);
    //     vTaskDelay(25 / portTICK_PERIOD_MS);
    //     gpio_set_low(GPIO_NUMBER_5);
    //     vTaskDelay(25 / portTICK_PERIOD_MS);
    // }