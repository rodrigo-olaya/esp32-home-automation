#include "nema17.h"

static uint8_t step_delay_in_msec = 50;
static float step_resolution = 1.8;

static gpio_number_t motor_step_gpio = GPIO_NUMBER_5;
static gpio_number_t motor_dir_gpio = GPIO_NUMBER_18;

static const char* MOTOR_TAG = "motor";

void set_step_resolution() {}

void set_motor_speed(uint8_t target_delay_in_msec){
    step_delay_in_msec = target_delay_in_msec;
}

void set_direction(uint8_t direction){
    gpio_set_dir(motor_step_gpio, direction);  // STEP
    gpio_set_dir(motor_dir_gpio, direction); // DIR
}

void step() {
    gpio_set_high(motor_step_gpio);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
    gpio_set_low(motor_step_gpio);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
}

void move_to_angle(float target_angle) {
    ESP_LOGI(MOTOR_TAG, "moving to requested angle");
    set_direction(GPIO_OUTPUT);

    int target_steps = calculate_steps(target_angle, step_resolution);

    ESP_LOGI(MOTOR_TAG, "Target steps: %d", target_steps);

    for (int i = 0; i < target_steps; i++){
        ESP_LOGI(MOTOR_TAG, "Stepping");

        step();
    }
}