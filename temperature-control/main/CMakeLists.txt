idf_component_register(SRCS "main.c"
                            "sensors/thermistor.c"
                            "drivers/gpio_config.c"
                            "communication/queues.c"
                            "control/control.c"
                            "MQTT/publisher.c"
                            "WiFi/WiFi.c"
                            "drivers/gpio_driver.c"
                            "drivers/i2c_driver.c"
                            "drivers/onewire.c"
                            "motors/nema17.c"
                            "motors/encoder.c"
                            "motors/torque_test.c"
                            "motors/handlers/calculate_steps.c"
                            "motors/handlers/calculate_angle.c"
                            "data/handle_temperature.c"
                            "data/handle_humidity.c"
                            "sensors/humidity_sensor.c"
                       PRIV_REQUIRES spi_flash mqtt nvs_flash esp_netif esp_wifi
                       INCLUDE_DIRS "config")

# Motor switch bench test, built separately so the normal build is untouched:
#   idf.py -B test-build -D MOTOR_SWITCH_TEST=1 flash monitor
if(MOTOR_SWITCH_TEST)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE MOTOR_SWITCH_TEST)
endif()
