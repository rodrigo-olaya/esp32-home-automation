#include "calculate_angle.h"

double calculate_angle(uint8_t angle_high, uint8_t angle_low) {
    uint16_t raw_value = ((angle_high << 8) | angle_low) & 0x0FFF;
    double degrees = (raw_value / 4096.0) * 360.0;
    return degrees;
}