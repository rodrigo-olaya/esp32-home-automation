#include "handle_temperature.h"

double convert_to_farenheit(double celsius) {
    return celsius * 9/5 + 32;
}

int round_temperature(double farenheit){
    return round(farenheit);
}

float raw_onewire_to_temp(uint8_t msb, uint8_t lsb) {
    int16_t raw = (msb << 8) | lsb;
    return raw / 16.0;    
}