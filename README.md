# esp32-home-automation
Project to make my home smart using several ESP32 MCUs, sensors, and motors.

## Main Components
This Smart Home system has the following features:
- Temperature Monitoring
- Temperature Control
- Humidity Monitoring
- Security Camera

### Temperature Monitoring
Temperature is monitored using a DS18B20 sensor, which uses the 1-Wire protocol to communicate with an ESP32.

### Temperature Control
Temperature is controled using a NEMA17 motor. In order to prevent step loss, and ensure the motor sets the switch to the target temperature, a magnetic encoder is used.

### Humidity Monitoring
Humidity is monitored using an SHT31 sensor, which uses I2C to communicate with ESP32
