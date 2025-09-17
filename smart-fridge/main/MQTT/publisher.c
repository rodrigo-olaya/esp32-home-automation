#include "publisher.h"

void publish_data() {
    mosquitto_lib_init();

    const char id = "mqtt_client";

    struct mosquitto *mosquitto_ret = mosquitto_new(&id, true, NULL);
    if (mosquitto_ret == NULL) {
        // clean up if mosquitto client instance creation fails
        mosquitto_lib_cleanup();
    }

    mosquitto_connect();

    mosquitto_publish();

    mosquitto_disconnect();

    mosquitto_destroy();

    mosquitto_lib_cleanup();
}
