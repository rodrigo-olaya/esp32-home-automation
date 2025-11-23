#include "i2c_driver.h"

void i2c_init(){
    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    ets_delay_us(10);
}
void i2c_start(){
    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    ets_delay_us(10);

    gpio_set_dir(I2C_SDA_GPIO, GPIO_OUTPUT);
    gpio_set_low(I2C_SDA_GPIO);
    ets_delay_us(10);

    gpio_set_dir(I2C_SCL_GPIO, GPIO_OUTPUT);
    gpio_set_low(I2C_SCL_GPIO);
    ets_delay_us(10);
}
void i2c_stop(){
    gpio_set_dir(I2C_SDA_GPIO, GPIO_OUTPUT);
    gpio_set_low(I2C_SDA_GPIO);
    ets_delay_us(10);

    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    ets_delay_us(10);
    
    gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    ets_delay_us(10);
}
i2c_return_t i2c_write_byte(uint8_t data){}
uint8_t i2c_read_byte(bool send_ack){}