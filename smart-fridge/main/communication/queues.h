#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "../sensors/sensor_defines.h"

/**
 * @brief This function initializes the queues to communicate from sensors to HomeAssistant
 * @param sensor_data
 */
uint8_t queues_init();