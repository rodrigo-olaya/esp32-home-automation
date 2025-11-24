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
i2c_return_t i2c_write_byte(uint8_t data){

    for (int i = 0; i < 8; i++){
        uint8_t bit = (data >> (7 - i)) & 0x01;
        if (bit == HIGH) {
            gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
        }
        else if (bit == LOW) {
            gpio_set_dir(I2C_SDA_GPIO, GPIO_OUTPUT);
            gpio_set_low(I2C_SDA_GPIO);
        }
        ets_delay_us(I2C_DELAY_US);

        gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
        ets_delay_us(I2C_DELAY_US);

        gpio_set_dir(I2C_SCL_GPIO, GPIO_OUTPUT);
        gpio_set_low(I2C_SCL_GPIO);
        ets_delay_us(10);
    }
    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    ets_delay_us(10);

    gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    ets_delay_us(10);
    i2c_return_t ack = gpio_read(I2C_SDA_GPIO);
    gpio_set_dir(I2C_SCL_GPIO, GPIO_OUTPUT);
    gpio_set_low(I2C_SCL_GPIO);
    ets_delay_us(I2C_DELAY_US);
    
    return ack;
}
uint8_t i2c_read_byte(bool send_ack){
    return 0;
}