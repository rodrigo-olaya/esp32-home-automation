#include "../config/private_config.h"
#include "string.h"
#include "mqtt_client.h"
#include "esp_event.h"

#define MQTT_PAYLOAD "27"
#define PI4_TOPIC "home/esp32/livingroom/temp"

void publish_data();
