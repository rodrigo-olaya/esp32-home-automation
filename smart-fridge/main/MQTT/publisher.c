#include "publisher.h"

void publish_data() {
    mosquitto_lib_init();

    const char* id = "mqtt_client";

    struct mosquitto *mosquitto_ret = mosquitto_new(id, true, NULL);
    if (mosquitto_ret == NULL) {
        // clean up if mosquitto client instance creation fails
        mosquitto_lib_cleanup();
    }

    int mosquitto_connection = mosquitto_connect(mosquitto_ret, MQTT_BROKER_IP, MQTT_PORT, 60);

    int publish_ret = mosquitto_publish(mosquitto_ret, NULL, "home/esp32/livingroom/temp", strlen(MQTT_PAYLOAD), MQTT_PAYLOAD, 0, false);

    mosquitto_disconnect(mosquitto_ret);

    mosquitto_destroy(mosquitto_ret);

    mosquitto_lib_cleanup();
}
