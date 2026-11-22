# temperature-control

ESP32 node that reads temperature and humidity and publishes them over MQTT.

The old stepper-motor thermostat control is retired and lives in [legacy/motor](legacy/motor).

## What it does

- **Temperature:** DS18B20 on 1-Wire (GPIO4).
- **Humidity:** SHT31 over I2C (address `0x44`).
- **Firmware structure:** sensor tasks push readings into a FreeRTOS queue, and a
  receiver task publishes them to MQTT. GPIO, I2C and 1-Wire are small bit-banged
  drivers in `main/drivers`.

## MQTT topics

| Topic | Direction | Payload |
|---|---|---|
| `home/esp32/<location>/temp` | publish | temperature in °F |
| `home/esp32/<location>/humidity` | publish | relative humidity |

`<location>` is per sensor, not per board: `SHT31_LOCATION` and `DS18B20_LOCATION` in
`main/config/private_config.h` (for example `bedroom` and `fridge`).

## Build and flash

Requires [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html) 5.5 at `~/esp/v5.5/esp-idf` (override with `IDF_EXPORT=...`).

```
cp main/config/private_config.h.example main/config/private_config.h   # then edit
make build
make flash    # flashes, then opens the monitor (Ctrl+] quits); PORT=... overrides the default port
```

`private_config.h` holds the WiFi credentials and broker address and is gitignored.

## Tests

Host-side unit tests (CppUTest) for the temperature and humidity conversions, no board needed:

```
make test
```

## Hardware

Carrier board: [../hardware/esp32_carrier_board](../hardware/esp32_carrier_board).
Design notes are in [requirements/](requirements/).
