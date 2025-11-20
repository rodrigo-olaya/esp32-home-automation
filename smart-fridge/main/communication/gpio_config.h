#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define FF_THERMISTOR_GPIO 4
#define I2C_SDA_GPIO GPIO_NUM_21
#define I2C_SCL_GPIO GPIO_NUM_22

#endif // GPIO_CONFIG_H