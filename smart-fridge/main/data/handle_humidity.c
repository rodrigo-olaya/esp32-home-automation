#include "handle_humidity.h"

double bytes_to_humidity(uint8_t humidity_msb, uint8_t humidity_lsb){
    uint16_t raw = (humidity_msb << 8) | humidity_lsb;
    double humidity = (raw / 65535.0) * 100;
    return round(humidity * 10.0) / 10.0;
}