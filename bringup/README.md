# Bring-up programs

Small throwaway programs that answer one question each about the hardware, in the order you'd build the board. Each folder is a tiny ESP-IDF project, built and flashed from here with the `Makefile`.

They use the stock ESP-IDF drivers on purpose: if one fails, the problem is the hardware or the wiring, not this repo's own drivers.

Needs ESP-IDF 5.5 at `~/esp/v5.5/esp-idf`. Override with `IDF_EXPORT=...`. Find the DevKit's port with `ls /dev/cu.*` (the DevKit shows up as `usbserial`), then pass it with `PORT=...` if it isn't `/dev/cu.usbserial-0001`.

```
make id                   # chip + flash info over USB, writes nothing
make flash-chip-check     # build, flash, open the serial monitor (Ctrl+] quits)
```

If flashing fails with "Connecting......", hold the DevKit's BOOT button while it connects.

## Programs

| Program | Question | Good output |
|---|---|---|
| `make id` | Does the DevKit answer over USB? | `Chip is ESP32-D0WD-V3`, a MAC address, a flash size |
| `chip-check` | Does it run code, and does GPIO2 work? | chip model, flash size, MAC, `reset reason: power on`, and the DevKit LED toggling. On the carrier board D5 blinks too. |
| `i2c-scan` | Is the I2C bus healthy, and what's on it? | `SDA idle: high (ok)`, `SCL idle: high (ok)`, then `0x44 SHT3x` once the SHT31 is wired to GPIO21/22 |
| `sht31-read` | Does the SHT31 give sane readings? | `T = 22.xx C ... RH = 4x %` with no CRC errors |

Reading the failures:
- `reset reason: BROWNOUT`: the supply sagged. Try another USB cable or port.
- `SDA/SCL idle: LOW`: that line is shorted to GND or something is holding it down.
- `found 0 device(s)` with both lines high: wrong pins, swapped SDA/SCL, or the sensor has no power.
- `CRC mismatch`: noisy or long wires, or a bad sensor.

## Pins used

| Signal | GPIO | Carrier board |
|---|---|---|
| Status LED | 2 | D5 (also the DevKit's own LED) |
| I2C SDA / SCL | 21 / 22 | J5, J6, J7 |

## Still to write (when the parts arrive)

| Program | Checks |
|---|---|
| `encoder-check` | KY-040 on GPIO25/26/27: rotation direction and the click, printed as events |
| `oled-check` | SSD1306 at `0x3C`: draws a test pattern |
| `buzzer-check` | passive buzzer on GPIO33: a short tone sweep |
