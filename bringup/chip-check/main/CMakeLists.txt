idf_component_register(SRCS "chip_check.c"
                       PRIV_REQUIRES esp_driver_gpio spi_flash
                       INCLUDE_DIRS ".")
