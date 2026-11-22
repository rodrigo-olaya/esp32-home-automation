idf_component_register(SRCS "main.c"
                            "sensors/ds18b20.c"
                            "drivers/gpio_config.c"
                            "communication/queues.c"
                            "control/control.c"
                            "MQTT/publisher.c"
                            "WiFi/WiFi.c"
                            "drivers/gpio_driver.c"
                            "drivers/i2c_driver.c"
                            "drivers/onewire.c"
                            "data/handle_temperature.c"
                            "data/handle_humidity.c"
                            "sensors/sht31.c"
                       PRIV_REQUIRES spi_flash mqtt nvs_flash esp_netif esp_wifi
                       INCLUDE_DIRS "config")

