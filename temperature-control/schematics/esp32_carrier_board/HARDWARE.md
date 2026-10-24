# ESP32 Home Node — rev B hardware interface

General-purpose carrier for a 30-pin ESP32 DevKit V1. Every section is optional: populate only what a
node needs and enable the matching features in firmware.

Board: 100 × 79 mm, 2 layers, all through-hole. Fab files: `fab/esp32_carrier_board_gerbers.zip`.

## Power

| Rail | Source | Feeds | Notes |
|---|---|---|---|
| +12V | J1 barrel jack (5.5×2.1 mm, center +) or J2 screw terminal, through D1 reverse-polarity diode | A4988 VMOT, U2, J8 12V OUT | 9–15 V. Optional. |
| 5V_REG | U2 TSR 1-2450 (1 A) | D3 | Only present when +12V is. |
| +5V | U2 through D3, **or** the DevKit's USB through its VIN pin | DevKit VIN, J9/J11 5V pins, power LED | ~4.6 V on 12 V (D3 drop). On USB only: limited to what the USB port supplies. |
| +3V3 | DevKit's on-board regulator (pin 30) | sensors, pull-ups, A4988 logic, J4/J5/J6/J7/J10/J11 | Keep total 3V3 load under ~300 mA. |

**USB-only nodes** (no 12 V): everything except the motor and J8 works. Confirm once per DevKit that
VIN reads ~4.7 V with only USB plugged in; most DevKits have the diode that makes this work.

## GPIO map

| GPIO | Net | Goes to | Direction | Notes |
|---|---|---|---|---|
| 4 | ONEWIRE | J4 pin 2, R2 4.7k pull-up, TP 1W | bidir | DS18B20 bus |
| 5 | STEP | A4988 STEP, TP STEP | out | Strapping pin (harmless here). STEP high ≥ 1 µs. |
| 18 | DIR | A4988 DIR, TP DIR | out | High = `CLOCKWISE` in `nema17.c` |
| 19 | MOTOR_EN_N | A4988 ~ENABLE | out | **Active low**. Carrier pulls it low, so the motor is enabled unless firmware drives it high. |
| 21 | SDA | J5/J6/J7 pin 4, R3 4.7k, TP SDA | bidir | One I2C bus, three ports |
| 22 | SCL | J5/J6/J7 pin 3, R4 4.7k, TP SCL | out | |
| 23 | OUT12_CTRL | Q1 gate via R6 100 Ω | out | **High = J8 output on.** R7 10k holds it off during boot. |
| 2 | STATUS_LED | D5 via R8 1k | out | High = on. Also the DevKit's own LED. |
| 36, 39, 34, 35 | IO36/39/34/35 | J11 pins 1–4 | **input only** | No internal pull-ups. ADC1: analog reads work with WiFi on. |
| 32 | IO32 | J11 pin 5 | bidir | ADC1 capable |
| 33 | SPI_MISO | J10 pin 1 | in | |
| 25 | SPI_CS | J10 pin 2 | out | |
| 26 | SPI_SCK | J10 pin 3 | out | |
| 27 | SPI_MOSI | J10 pin 4 | out | |
| 14 | UART_RX | J9 pin 3 | in | Use UART1/2 via the GPIO matrix. GPIO14 toggles briefly at boot. |
| 13 | UART_TX | J9 pin 4 | out | |
| 0, 12, 15 | — | unused | | Strapping pins, deliberately left free |
| 1, 3 | — | USB console | | |
| 16, 17 | — | unused | | Free for a future revision |

SPI and UART pins are ordinary GPIOs when not used for that purpose. ESP32 SPI over the GPIO matrix
tops out around 26 MHz, plenty for displays, SD cards and sensors.

## Connectors

Pin 1 is marked on the silkscreen (square pad / triangle).

| Ref | Label | Type | Pins (1 → n) |
|---|---|---|---|
| J1 | 12V IN | 5.5×2.1 mm barrel jack | center +12V, sleeve GND |
| J2 | + − | 2-pin screw, 5.08 mm | +12V in, GND |
| J3 | MOTOR | 4-pin JST-XH | 2B, 2A, 1A, 1B (3D-printer cable order) |
| J4 | 1-WIRE | 3-pin screw, 5.08 mm | 3V3, DQ, GND (probe: red, yellow, black) |
| J5–J7 | I2C-1/2/3 | 4-pin JST-XH | 3V3, GND, SCL, SDA |
| J8 | 12V OUT | 2-pin screw, 5.08 mm | 1 = LOAD− (switched), 2 = +12V. Load + to pin 2, load − to pin 1. ~2 A max (shared with the motor and input supply). |
| J9 | UART | 1×4 header | 5V, GND, RX (GPIO14), TX (GPIO13) |
| J10 | SPI | 1×6 header | MISO (33), CS (25), SCK (26), MOSI (27), 3V3, GND |
| J11 | GPIO | 1×8 header | 36, 39, 34, 35, 32, 3V3, 5V, GND |
| JP1 | MICROSTEP | 2×3 header | Jumper ties MS1/MS2/MS3 high. No jumpers = full step. |

## I2C addresses in use

| Address | Device |
|---|---|
| 0x36 | AS5600 encoder |
| 0x44 | SHT3x humidity |

New I2C sensors can go on any port as long as their address is different.

## Population by role

| Role | Fit | Leave off |
|---|---|---|
| Motor node (stepper + encoder) | Everything in the power section, U3 socket, C1, C2, J3, JP1 (optional) | — |
| Sensor-only node (USB powered) | DevKit socket, pull-ups R2–R4, ports/headers you need, D5/R8 | J1, J2, D1, C1, C2, U2, C3, D3, U3 socket, J3, JP1, R5, Q1/R6/R7/D4/J8 |
| Switched 12 V load | Power section + Q1, R6, R7, D4, J8 | Motor section if unused |

## Bring-up checklist

1. Before any power: continuity check +12V–GND, +5V–GND, +3V3–GND. None should beep.
2. Power with no modules plugged in: ~4.6 V at the DevKit socket's VIN pin (12 V input), 0 V at J8.
3. Plug in the DevKit: 3.3 V at J11 pin 6, status LED responds to GPIO2.
4. A4988: plug in with the motor **already connected**, set Vref (≈0.75 V for a 1 A, 17HS4023-type motor
   with R100 sense resistors), never unplug the motor while powered.
5. J8: test with a 12 V LED strip before inductive loads.
