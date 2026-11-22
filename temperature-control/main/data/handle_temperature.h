#ifndef HANDLE_TEMPERATURE_H
#define HANDLE_TEMPERATURE_H

#include <math.h>
#include <stdint.h>

double convert_to_farenheit(double celsius);

int round_temperature(double farenheit);

float raw_onewire_to_temp(uint8_t msb, uint8_t lsb);

/**
 * @brief Converts the raw 16-bit SHT31 temperature word to degrees Celsius.
 * The datasheet formula is: T = -45 + 175 * raw / 65535. The word is unsigned.
 * @param msb first byte the sensor sent
 * @param lsb second byte the sensor sent
 */
float raw_sht31_to_temp(uint8_t msb, uint8_t lsb);

#endif