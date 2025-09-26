#include "../config/private_config.h"
#include "string.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"

#define PI4_TOPIC "home/esp32/livingroom/temp"

void publish_data(float dataToPublish);

void mqtt_init();
