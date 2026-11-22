#include "crc.h"
#include "string.h"

uint8_t crc8_sht31(const uint8_t *data, size_t len) {
    
    uint8_t crc;
    
    for (int i = 0; i < len; i++) {
        // if (i == 1) {
            //     crc = data[i] ^ crc; 
            // }
            for (int j = 0; j < 9; j++) {
                if (i == 0 && j == 0) {
                    crc = 0xFF ^ data[i];
                }
                else if (j == 0) {
                    crc = crc ^ data[i];
                }
                else {
                    if ((crc & 0x80) == 0x80) {
                        crc = (crc << 1) ^ 0x31;
                    }
                    else {
                        crc <<= 1;
                    }
                }
            }
        }
        
        return crc;
    }     // SHT3x: poly 0x31, init 0xFF, MSB first

uint8_t crc8_ds18b20(const uint8_t *data, size_t len) {
    
    uint8_t crc;

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 9; j++) {
            if (i == 0 && j == 0) {
                crc = 0x00 ^ data[i];
            }
            else if (j == 0) {
                crc = crc ^ data[i];
            }
            else {
                if ((crc & 0x01) == 0x01) {
                    crc = (crc >> 1) ^ 0x8C;
                }
                else {
                    crc >>= 1;
                }
            }
        }
    }
    
    return crc;
    
}  // Dallas/Maxim: poly 0x8C reflected, init 0
bool ds18b20_scratchpad_valid(const uint8_t scratchpad[9]) {
    static const uint8_t zeros[9] = {0};
    if (memcmp(scratchpad, zeros, 9) == 0) {
        return false;
    }
    uint8_t crc = crc8_ds18b20(scratchpad, 8);
    return crc == scratchpad[8];
}   