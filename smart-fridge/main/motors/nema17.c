#include "nema17.h"

static uint8_t step_delay_in_msec = 50;
static float step_resolution = 1.8;

static gpio_number_t motor_step_gpio = GPIO_NUMBER_5;
static gpio_number_t motor_dir_gpio = GPIO_NUMBER_18;

static const char* MOTOR_TAG = "motor";

void handle_motor_action(esp_mqtt_event_handle_t event) {
    char data[event->data_len + 1];
    memcpy(data, event->data, event->data_len);
    data[event->data_len] = '\0';
    if (strcmp(data,"heat") == 0){
        ESP_LOGI(MOTOR_TAG, "Configuring as heater");        
        turn_heater_on();
        
    }
    else if (strcmp(data,"cool") == 0){
        ESP_LOGI(MOTOR_TAG, "Configuring as cooler");
        set_direction(COUNTERCLOCKWISE);
    }
    else if (strcmp(data,"OFF") == 0){
        ESP_LOGI(MOTOR_TAG, "Configure AC OFF");
        turn_heater_off();
        
    }
}

void set_step_resolution() {}

void set_motor_speed(uint8_t target_delay_in_msec){
    step_delay_in_msec = target_delay_in_msec;
}

void set_direction(motor_direction_t direction){
    gpio_set_dir(motor_step_gpio, GPIO_OUTPUT);  // STEP
    gpio_set_dir(motor_dir_gpio, GPIO_OUTPUT); // DIR
    if (direction == CLOCKWISE){
        gpio_set_high(motor_dir_gpio);
    }
    else {
        gpio_set_low(motor_dir_gpio);
    }
    vTaskDelay(pdMS_TO_TICKS(1));
}

void step() {
    gpio_set_high(motor_step_gpio);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
    gpio_set_low(motor_step_gpio);
    vTaskDelay(step_delay_in_msec / portTICK_PERIOD_MS);
}

void move_to_angle(float target_angle) {
    ESP_LOGI(MOTOR_TAG, "moving to requested angle");

    int target_steps = calculate_steps(target_angle, step_resolution);

    ESP_LOGI(MOTOR_TAG, "Target steps: %d", target_steps);

    double current_angle = read_angle();
    // if (target_angle + current_angle >= 360){
    //     target_angle = target_angle + current_angle - 360;
    // }
    // else {
    //     target_angle += current_angle;
    // }

    ESP_LOGI(MOTOR_TAG, "Target angle: %f", target_angle);
    ESP_LOGI(MOTOR_TAG, "Angle: %f", current_angle);

    while (current_angle < target_angle - 10 || current_angle > target_angle + 10) {
        i2c_lock();
        step();
        current_angle = read_angle();
        ESP_LOGI(MOTOR_TAG, "Target angle: %f", target_angle);
        ESP_LOGI(MOTOR_TAG, "Angle: %f", current_angle);
        i2c_unlock();
    }
}

void turn_heater_on() {
    set_direction(CLOCKWISE);
    // move_to_angle(POSITION_RIGHT);
    move_to_angle(50.0);
}

void turn_heater_off() {
    set_direction(COUNTERCLOCKWISE);
    // move_to_angle(POSITION_LEFT);
    move_to_angle(350.0);
}