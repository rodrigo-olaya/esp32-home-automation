idf_component_register(SRCS "main.c"
                            "thermistor.c"
                       PRIV_REQUIRES spi_flash
                       INCLUDE_DIRS "")
