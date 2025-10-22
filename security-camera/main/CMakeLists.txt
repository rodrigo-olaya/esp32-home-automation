idf_component_register(SRCS "security-camera.c"
                            "WiFi/WiFi.c"
                            "camera/camera.c"
                            "MQTT/publisher.c"
                       PRIV_REQUIRES spi_flash mqtt nvs_flash esp_netif esp_wifi
                       INCLUDE_DIRS "config")

