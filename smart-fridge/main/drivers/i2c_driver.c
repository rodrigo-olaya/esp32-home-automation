#include "i2c_driver.h"

static SemaphoreHandle_t i2c_mutex = NULL;

void i2c_lock() {
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
}

void i2c_unlock() {
    xSemaphoreGive(i2c_mutex);
}

void i2c_release_serial_data() {
    gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    ets_delay_us(10);
}

void i2c_release_serial_clock() {
    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    ets_delay_us(10);
}

void i2c_pull_serial_data_low() {
    gpio_set_dir(I2C_SDA_GPIO, GPIO_OUTPUT);
    gpio_set_low(I2C_SDA_GPIO);
    ets_delay_us(10);
}

void i2c_pull_serial_clock_low() {
    gpio_set_dir(I2C_SCL_GPIO, GPIO_OUTPUT);
    gpio_set_low(I2C_SCL_GPIO);
    ets_delay_us(10);
}

void i2c_init(){
    i2c_mutex = xSemaphoreCreateMutex();
    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    ets_delay_us(10);
}
void i2c_start(){
    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    ets_delay_us(10);

    i2c_pull_serial_data_low();
    i2c_pull_serial_clock_low();
}
void i2c_stop(){
    i2c_pull_serial_data_low();
    i2c_release_serial_clock();
    i2c_release_serial_data();
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

        i2c_release_serial_clock();
        i2c_pull_serial_clock_low();
    }
    i2c_release_serial_clock();
    i2c_release_serial_data();

    i2c_return_t ack = gpio_read(I2C_SDA_GPIO);
    i2c_pull_serial_clock_low();
    
    return ack;
}

uint8_t i2c_read_byte(bool send_ack){
    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        i2c_release_serial_data();
        i2c_release_serial_clock();
        
        uint8_t bit = gpio_read(I2C_SDA_GPIO);
        data |= (bit << (7 - i));

        i2c_pull_serial_clock_low();
    }

    if (send_ack) {
        gpio_set_dir(I2C_SDA_GPIO, GPIO_OUTPUT);
        gpio_set_low(I2C_SDA_GPIO);
    } else {
        gpio_set_dir(I2C_SDA_GPIO, GPIO_INPUT);
    }

    ets_delay_us(10);
    gpio_set_dir(I2C_SCL_GPIO, GPIO_INPUT);
    ets_delay_us(I2C_DELAY_US);
    gpio_set_dir(I2C_SCL_GPIO, GPIO_OUTPUT);
    ets_delay_us(I2C_DELAY_US);
    gpio_set_low(I2C_SCL_GPIO);
    ets_delay_us(I2C_DELAY_US);

    return data;
}