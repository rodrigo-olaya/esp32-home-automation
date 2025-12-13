#ifndef GPIO_REGISTERS_H
#define GPIO_REGISTERS_H

#include <stdint.h>

#define GPIO_BASE               0x3ff44000
#define GPIO_OUT_REG            (*(volatile uint32_t*)(GPIO_BASE + 0x0004))
#define GPIO_OUT_W1TS_REG       (*(volatile uint32_t*)(GPIO_BASE + 0x0008))
#define GPIO_OUT_W1TC_REG       (*(volatile uint32_t*)(GPIO_BASE + 0x000C))
#define GPIO_ENABLE_REG         (*(volatile uint32_t*)(GPIO_BASE + 0x0020))
#define GPIO_ENABLE_W1TS_REG    (*(volatile uint32_t*)(GPIO_BASE + 0x0024))
#define GPIO_ENABLE_W1TC_REG    (*(volatile uint32_t*)(GPIO_BASE + 0x0028))
#define GPIO_ENABLE1_REG        (*(volatile uint32_t*)(GPIO_BASE + 0x002C))
#define GPIO_ENABLE1_W1TS_REG   (*(volatile uint32_t*)(GPIO_BASE + 0x0030))
#define GPIO_ENABLE1_W1TC_REG   (*(volatile uint32_t*)(GPIO_BASE + 0x0034))
#define GPIO_STRAP_REG          (*(volatile uint32_t*)(GPIO_BASE + 0x0038))
#define GPIO_IN_REG             (*(volatile uint32_t*)(GPIO_BASE + 0x003C))
#define GPIO_IN1_REG            (*(volatile uint32_t*)(GPIO_BASE + 0x0040))

#endif //GPIO_REGISTERS_H