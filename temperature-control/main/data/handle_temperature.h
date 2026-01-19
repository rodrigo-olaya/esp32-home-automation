#ifndef HANDLE_TEMPERATURE_H
#define HANDLE_TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

double convert_to_farenheit(double celsius);

int round_temperature(double farenheit);

float raw_onewire_to_temp(uint8_t msb, uint8_t lsb);

#ifdef __cplusplus
}
#endif

#endif