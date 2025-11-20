#ifndef QUEUES_H
#define QUEUES_H

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "../sensors/sensor_defines.h"

/**
 * @brief This function initializes the queues to communicate from sensors to HomeAssistant
 * @return 
 *      - 1 on success
 *      - 0 on failure
 */
uint8_t queues_init();

#endif // QUEUES_H