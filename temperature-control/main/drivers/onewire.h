#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <inttypes.h>
#include "rom/ets_sys.h"
#include "gpio_registers.h"
#include "gpio_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void onewire_init(int gpio_pin);

int onewire_reset(void);

void onewire_write_bit(int bit);

int onewire_read_bit(void);

void onewire_write_byte(uint8_t byte);

uint8_t onewire_read_byte(void);

#endif // ONEWIRE_H