#ifndef HANDLE_TEMPERATURE_H
#define HANDLE_TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

double convert_to_farenheit(double celsius);

int round_temperature(double farenheit);

#ifdef __cplusplus
}
#endif

#endif