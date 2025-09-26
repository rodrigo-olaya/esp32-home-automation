idf_component_register(SRCS "main.c"
                            "sensors/thermistor.c"
                            "communication/gpio_config.c"
                            "communication/queues.c"
                            "communication/sensors.c"
                            "control/control.c"
                            "MQTT/publisher.c"
                            "WiFi/WiFi.c"
                       PRIV_REQUIRES spi_flash mqtt nvs_flash esp_netif esp_wifi
                       INCLUDE_DIRS "config")
