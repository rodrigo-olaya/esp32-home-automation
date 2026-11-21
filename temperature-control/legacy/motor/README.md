# Legacy: thermostat motor

Code for pushing a wall thermostat's switch with a NEMA17 stepper (A4988 driver) and an
AS5600 magnetic encoder for position feedback. Retired because the landlord installed a
thermostat UI, so the node no longer actuates the heat. Kept for when the motor finds a new
use. **Nothing here is compiled.**

| Path | What it is |
|---|---|
| `nema17.[ch]` | A4988 STEP/DIR driving, `turn_heater_on/off`, `move_to_angle`, `handle_motor_action` (MQTT command handler) |
| `encoder.[ch]` | AS5600 angle read over I2C |
| `handlers/` | Pure math: `calculate_steps`, `calculate_angle` |
| `torque_test.[ch]` | Keyboard-driven motor and encoder bench test |
| `tests/` | CppUTest tests for `handlers/` (they need `../test/main_tdd.cpp` as the runner) |
| `requirements/thermostatMotor.md` | Original design notes |

## How it was wired in

To bring it back, move the files into `main/motors/` (the `#include "../drivers/..."` paths
assume that location) and restore:

- **`main/CMakeLists.txt`:** the five `motors/...` sources, plus
  `if(MOTOR_SWITCH_TEST) target_compile_definitions(${COMPONENT_LIB} PRIVATE MOTOR_SWITCH_TEST) endif()`
  for the bench build (`idf.py -B test-build -D MOTOR_SWITCH_TEST=1 flash monitor`).
- **`main/main.c`:** `#ifdef MOTOR_SWITCH_TEST` → `torque_test_console();` at the top of `app_main`.
- **`main/MQTT/publisher.[ch]`:** `HEATER_TOPIC` (`home/esp32/controls/AC`, payloads `heatHIGH`,
  `heatOFF`, `cool`), a subscribe in `MQTT_EVENT_CONNECTED`, and a call to
  `handle_motor_action(event)` in `MQTT_EVENT_DATA`.
- **`test/Makefile`:** the two `calculate_*` sources and test files.

Hardware for this lives on the carrier board's stage 3 (A4988, 12 V, J3).
