#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

uint8_t crc8_sht31(const uint8_t *data, size_t len);     // SHT3x: poly 0x31, init 0xFF, MSB first
uint8_t crc8_ds18b20(const uint8_t *data, size_t len);   // Dallas/Maxim: poly 0x8C reflected, init 0
bool ds18b20_scratchpad_valid(const uint8_t scratchpad[9]);
// true only for a real scratchpad: CRC over all 9 bytes is 0 AND it is not what a dead bus reads.

#endif /* CRC_H */