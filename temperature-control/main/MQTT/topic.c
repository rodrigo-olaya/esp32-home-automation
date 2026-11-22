#include "topic.h"

bool build_topic(char *out, size_t out_size, const char *location, sensorType_t type) {
    if (location == NULL || !strcmp(location, "")) {
        return false;
    }
    
    if (out_size < strlen(location) + strlen(type == 0 ? "temp" : "humidity") + strlen("home/esp32//") + 1) {
        return false;
    }

    snprintf(out, out_size, "home/esp32/%s/%s", location, (type == 0 ? "temp" : "humidity"));

    return true;
}