# security-camera

ESP32-CAM node that captures JPEG frames and publishes them over MQTT.

## MQTT topics

| Topic | Direction | Purpose |
|---|---|---|
| `home/esp32/camera` | publish | camera frames |
| `home/esp32/controls/camera` | subscribe | camera control |
| `home/esp32/scanner/images` | publish | frames for the barcode scanner |
| `home/esp32/scanner/control` | subscribe | barcode scanner control |

## Build and flash

Requires [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html) 5.x.
The `esp32-camera` component is fetched automatically by the component manager.

```
cp main/config/private_config.h.example main/config/private_config.h   # then edit
idf.py build
idf.py -p <port> flash monitor
```

`private_config.h` holds the WiFi credentials and broker address and is gitignored.
