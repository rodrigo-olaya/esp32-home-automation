#include "../config/private_config.h"
#include "string.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_camera.h"
#include "../camera/camera.h"

#define CAM_TOPIC "home/esp32/camera"
#define CONTROL_TOPIC "home/esp32/controls/camera"

/**
 * @brief This function publishes data to the MQTT broker in Raspberry Pi
 * @param sensor_data struct that holds sensor type and sensor data
 */
void publish_data( camera_fb_t *fb);

/**
 * @brief This function initializes MQTT communication
 */
void mqtt_init();
