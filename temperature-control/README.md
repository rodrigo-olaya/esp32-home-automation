# temperature-control

ESP32 node that reads temperature and humidity, publishes them over MQTT, and
turns a wall thermostat on or off with a stepper motor on command.

## What it does

- **Temperature:** DS18B20 on 1-Wire (GPIO4).
- **Humidity:** SHT31 over I2C (address `0x44`).
- **Heater control:** a NEMA17 driven by an A4988 (STEP GPIO5, DIR GPIO18) pushes the
  thermostat switch. An AS5600 magnetic encoder gives position feedback so a lost
  step doesn't leave the switch half-pushed.
- **Firmware structure:** sensor tasks push readings into a FreeRTOS queue, and a
  receiver task publishes them to MQTT. GPIO, I2C and 1-Wire are small bit-banged
  drivers in `main/drivers`.

## MQTT topics

| Topic | Direction | Payload |
|---|---|---|
| `home/esp32/livingroom/temp` | publish | temperature |
| `home/esp32/livingroom/humidity` | publish | relative humidity |
| `home/esp32/controls/AC` | subscribe | `heatHIGH`, `heatOFF`, `cool` |

## Build and flash

Requires [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html) 5.x.

```
cp main/config/private_config.h.example main/config/private_config.h   # then edit
idf.py build
idf.py -p <port> flash monitor
```

`private_config.h` holds the WiFi credentials and broker address and is gitignored.

## Tests

Host-side unit tests (CppUTest) for the conversion and motor math:

```
cd test && make test
```

## Motor bench test

Keyboard-driven motor and encoder test, no WiFi or MQTT. It builds separately so the
normal build isn't touched:

```
idf.py -B test-build -D MOTOR_SWITCH_TEST=1 flash monitor
```

## Hardware

Carrier board: [../hardware/esp32_carrier_board](../hardware/esp32_carrier_board).
Design notes are in [requirements/](requirements/).
