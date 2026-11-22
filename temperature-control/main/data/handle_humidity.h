#ifndef HANDLE_HUMIDITY_H
#define HANDLE_HUMIDITY_H

#include <math.h>
#include <inttypes.h>

double bytes_to_humidity(uint8_t humidity_msb, uint8_t humidity_lsb);

#endif