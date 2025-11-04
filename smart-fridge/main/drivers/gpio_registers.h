#ifndef GPIO_REGISTERS_H
#define GPIO_REGISTERS_H

#include <stdint.h>

#define GPIO_BASE           0x3ff44000
#define GPIO_OUT_REG        (*(volatile uint32_t*)(GPIO_BASE + 0x0004))
#define GPIO_OUT_W1TS_REG   (*(volatile uint32_t*)(GPIO_BASE + 0x0008))
#define GPIO_OUT_W1TC_REG   (*(volatile uint32_t*)(GPIO_BASE + 0x000C))
#define GPIO_ENABLE_REG     (*(volatile uint32_t*)(GPIO_BASE + 0x0020))

#endif //GPIO_REGISTERS_H