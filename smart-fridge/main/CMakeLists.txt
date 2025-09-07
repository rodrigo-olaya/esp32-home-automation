idf_component_register(SRCS "main.c"
                            "sensors/thermistor.c"
                            "communication/gpio_config.c"
                            "communication/queues.c"
                            "communication/sensors.c"
                            "control/control.c"
                       PRIV_REQUIRES spi_flash
                       INCLUDE_DIRS "")
