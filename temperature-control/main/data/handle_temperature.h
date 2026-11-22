#ifndef HANDLE_TEMPERATURE_H
#define HANDLE_TEMPERATURE_H

#include <math.h>
#include <stdint.h>

double convert_to_farenheit(double celsius);

int round_temperature(double farenheit);

float raw_onewire_to_temp(uint8_t msb, uint8_t lsb);

#endif