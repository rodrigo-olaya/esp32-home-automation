// #include "mosquitto.h"
#include "../config/private_config.h"
#include "string.h"
#include "mqtt_client.h"

#define MQTT_PAYLOAD "23.5"
#define PI4_TOPIC "home/esp32/livingroom/temp"

void publish_data();
