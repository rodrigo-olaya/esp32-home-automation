#include "torque_test.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include "../drivers/gpio_driver.h"
#include "../drivers/i2c_driver.h"
#include "encoder.h"

static const char *TORQUE_TAG = "torque_test";

static const gpio_number_t step_gpio = GPIO_NUMBER_5;
static const gpio_number_t dir_gpio = GPIO_NUMBER_18;

#define STEP_DEGREES    1.8     // full step: no microstep jumpers fitted
#define STEP_PERIOD_MS  50      // 20 steps/s: slow, so the motor is near its strongest
#define PRESS_STEPS     20      // 36 degrees
#define SLIP_TOLERANCE  3.6     // degrees (2 steps) of disagreement before calling it a stall

#define AS5600_ADDR     0x36

static int last_press_steps = 0;
static bool last_press_clockwise = true;
static bool encoder_present = false;

static void step_n(int steps, bool clockwise) {
    if (clockwise) {
        gpio_set_high(dir_gpio);    // same convention as set_direction(CLOCKWISE)
    }
    else {
        gpio_set_low(dir_gpio);
    }
    ets_delay_us(5);    // A4988: DIR must settle >= 200 ns before a STEP edge

    for (int i = 0; i < steps; i++) {
        gpio_set_high(step_gpio);
        ets_delay_us(5);    // A4988: STEP must stay high >= 1 us
        gpio_set_low(step_gpio);
        vTaskDelay(pdMS_TO_TICKS(STEP_PERIOD_MS));
    }
}

/* True if the AS5600 acknowledges its address. Asked twice so one noisy read can't fake it. */
static bool encoder_probe(void) {
    int answers = 0;
    for (int i = 0; i < 2; i++) {
        i2c_lock();
        i2c_start();
        if (i2c_write_byte(AS5600_ADDR << 1) == ACK) {
            answers++;
        }
        i2c_stop();
        i2c_unlock();
    }
    return answers == 2;
}

static double encoder_angle(void) {
    i2c_lock();
    double angle = read_angle();
    i2c_unlock();
    return angle;
}

/* Smallest signed difference between two angles, in (-180, 180] */
static double angle_delta(double from, double to) {
    double d = fmod(to - from + 540.0, 360.0) - 180.0;
    return d;
}

static void move_and_report(int steps, bool clockwise) {
    if (!encoder_present) {
        step_n(steps, clockwise);
        ESP_LOGI(TORQUE_TAG, "%s %d step(s): commanded %.1f deg (no encoder: watch whether the arm actually moved)",
                 clockwise ? "CW " : "CCW", steps, steps * STEP_DEGREES);
        return;
    }

    double before = encoder_angle();
    step_n(steps, clockwise);
    vTaskDelay(pdMS_TO_TICKS(200));     // let the shaft settle before reading
    double after = encoder_angle();

    double commanded = steps * STEP_DEGREES;
    double measured = fabs(angle_delta(before, after));
    bool stalled = (commanded - measured) > SLIP_TOLERANCE;

    ESP_LOGI(TORQUE_TAG, "%s %d step(s): commanded %.1f deg, encoder moved %.1f deg (%.1f -> %.1f)  %s",
             clockwise ? "CW " : "CCW", steps, commanded, measured, before, after,
             stalled ? "<-- STALLED: motor could not complete the move" : "OK");
}

static void print_help(void) {
    printf("\n=== Motor switch test ===\n"
           "  d / a : jog 1 step   clockwise / counter-clockwise\n"
           "  D / A : jog 10 steps clockwise / counter-clockwise\n"
           "  p / P : PRESS %d steps clockwise / counter-clockwise, then hold\n"
           "  r     : return the last press\n"
           "  ?     : show this help\n"
           "Exit the monitor with Ctrl+]\n\n", PRESS_STEPS);
}

void torque_test_console(void) {
    gpio_set_dir(step_gpio, GPIO_OUTPUT);
    gpio_set_dir(dir_gpio, GPIO_OUTPUT);
    gpio_set_low(step_gpio);

    i2c_init();
    encoder_present = encoder_probe();
    if (encoder_present) {
        ESP_LOGI(TORQUE_TAG, "Encoder found. Angle at start: %.1f deg", encoder_angle());
    }
    else {
        ESP_LOGW(TORQUE_TAG, "No encoder found: the motor still runs, judge stalls by eye");
    }

    setvbuf(stdin, NULL, _IONBF, 0);    // hand us each key as soon as it is typed
    print_help();

    for (;;) {
        int c = fgetc(stdin);
        if (c == EOF) {
            clearerr(stdin);
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        switch (c) {
            case 'd': move_and_report(1, true); break;
            case 'a': move_and_report(1, false); break;
            case 'D': move_and_report(10, true); break;
            case 'A': move_and_report(10, false); break;
            case 'p':
            case 'P':
                last_press_clockwise = (c == 'p');
                last_press_steps = PRESS_STEPS;
                move_and_report(PRESS_STEPS, last_press_clockwise);
                break;
            case 'r':
                if (last_press_steps > 0) {
                    move_and_report(last_press_steps, !last_press_clockwise);
                    last_press_steps = 0;
                }
                else {
                    ESP_LOGI(TORQUE_TAG, "Nothing to return");
                }
                break;
            case '?': print_help(); break;
            default: break;     // ignore newlines and other keys
        }
    }
}
