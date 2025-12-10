#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "gpio_config.h"
#include "gpio_driver.h"
#include "rom/ets_sys.h"

#define I2C_DELAY_US 5

typedef enum {
    ACK = 0,
    NACK = 1
} i2c_return_t;

void i2c_lock();
void i2c_unlock();

void i2c_release_serial_data();
void i2c_release_serial_clock();
void i2c_pull_serial_data_low();
void i2c_pull_serial_clock_low();

void i2c_init();
void i2c_start();
void i2c_stop();
i2c_return_t i2c_write_byte(uint8_t data);
uint8_t i2c_read_byte(bool send_ack);
// void i2c_bus_reset();

#endif //I2C_DRIVER_H