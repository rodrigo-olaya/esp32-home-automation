#include "publisher.h"

void publish_data() {
    mosquitto_lib_init();

    mosquitto_new();

    mosquitto_connect();

    mosquitto_publish();

    mosquitto_disconnect();

    mosquitto_destroy();

    mosquitto_lib_cleanup();
}
