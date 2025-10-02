# include "SHT_3x.h"

static const char *SHT3X_TAG = "SHT3x";
sht3x_t humidity_sensor_desc;

void humidity_sensor_init() {

    uint8_t sht3x_addr = 0x44;

    ESP_ERROR_CHECK(i2cdev_init());

    esp_err_t init_desc_ret = sht3x_init_desc(&humidity_sensor_desc, sht3x_addr, I2C_NUM_0, I2C_SDA_GPIO, I2C_SCL_GPIO);
    if (init_desc_ret != ESP_OK){
        ESP_LOGE(SHT3X_TAG, "Device Desc init failed");
    }

    esp_err_t init_ret = sht3x_init(&humidity_sensor_desc);
    if (init_ret != ESP_OK){
        ESP_LOGE(SHT3X_TAG, "Init failed");
    }

    vTaskDelay(pdMS_TO_TICKS(100));
}

void read_humidity() {

    humidity_sensor_init();

    float temperature;
    float humidity;

    for( ;; ) {
        esp_err_t measure_ret = sht3x_measure(&humidity_sensor_desc, &temperature, &humidity);
        if (measure_ret != ESP_OK){
            ESP_LOGE(SHT3X_TAG, "Measurement failed");
        }

        ESP_LOGI(SHT3X_TAG, "Temperature: %f", temperature);
        ESP_LOGI(SHT3X_TAG, "Humidity: %f", humidity);

        sensor_send_data(&humidity);

        vTaskDelay(pdMS_TO_TICKS(10000));

        // esp_err_t free_ret = sht3x_free_desc(&humidity_sensor_desc);
        // if (free_ret != ESP_OK){
        //     ESP_LOGE(SHT3X_TAG, "Descriptor free failed");
        // }
    }

    
}