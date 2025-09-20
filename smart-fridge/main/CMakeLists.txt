idf_component_register(SRCS "main.c"
                            "sensors/thermistor.c"
                            "communication/gpio_config.c"
                            "communication/queues.c"
                            "communication/sensors.c"
                            "control/control.c"
                            "MQTT/publisher.c"
                       PRIV_REQUIRES spi_flash json spiffs mosquitto
                       INCLUDE_DIRS "config")
