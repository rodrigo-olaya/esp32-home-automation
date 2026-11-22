idf_component_register(SRCS "i2c_scan.c"
                       PRIV_REQUIRES esp_driver_gpio esp_driver_i2c
                       INCLUDE_DIRS ".")
