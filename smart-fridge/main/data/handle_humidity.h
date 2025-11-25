#ifndef HANDLE_HUMIDITY_H
#define HANDLE_HUMIDITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <inttypes.h>

float bytes_to_humidity(uint8_t humidity_msb, uint8_t humidity_lsb);

#ifdef __cplusplus
}
#endif

#endif