# Thermostat Motor Control Requirements

## 1. System Overview

### Purpose
The purpose of this motor control is to set a thermostat to a certain temperature. For now, the goal is to activate heating by setting temperature to 70˚F and deactivate it by setting the temperature very low (50˚F).

### Physical Setup
This system controls a thermostat with an analog temperature dial. A NEMA 17 stepper motor with an attached handle physically rotates the dial to set temperature setpoints.

### Mechanical Constraint
The temperature dial is a switch that must be pushed in a specific direction to change state:
- To activate heating: Push switch RIGHT (clockwise)
- To deactivate heating: Push switch LEFT (counter-clockwise)

The motor handle must be positioned on the opposite side of the switch before pushing, requiring a two-step movement for each command.

### Control Method
Commands are received via MQTT. An AS5600 magnetic encoder provides closed-loop position feedback.

## 2. Hardware Setup & Position Definitions

### Encoder Positions
- **Heat ON position:** 50° (encoder angle) → thermostat set to ~70°F
- **Heat OFF position:** 350° (encoder angle) → thermostat set to ~50°F

### Position States
The system maintains two operational states based on handle position:

**POSITION_RIGHT (50°):**
- Handle is on the right side of the switch
- Ready to push LEFT for heat off command
- This is the assumed starting position on boot

**POSITION_LEFT (350°):**
- Handle is on the left side of the switch  
- Ready to push RIGHT for heat on command

### Movement Distances
- **Short path (activation push):** 60° (either 350°→50° or 50°→350°)
- **Long path (repositioning):** 300° (going the opposite direction around)

## 3. Command Sequences

### Heat ON Command
**MQTT Topic:** home/esp32/controls/AC
**MQTT Payload:** "heat"

**Precondition:** Motor is at POSITION_LEFT (350°)

**Sequence:**
1. **Push Phase:** Move clockwise from 350° to 50° (60° movement)
   - Direction: CLOCKWISE
   - Target: 50°
   - Action: Pushes switch RIGHT, activating heating
2. **Delay:** Wait 500ms for switch to settle
3. **Reposition Phase:** Move counter-clockwise from 50° to 350° (300° movement)
   - Direction: COUNTERCLOCKWISE
   - Target: 350°
   - Action: Prepares handle on left side for next heat off command

**Postcondition:** Motor is at POSITION_LEFT (350°), ready for heat off

---

### Heat OFF Command
**MQTT Topic:** home/esp32/controls/AC
**MQTT Payload:** "OFF"

**Precondition:** Motor is at POSITION_RIGHT (50°)

**Sequence:**
1. **Push Phase:** Move counter-clockwise from 50° to 350° (300° movement)
   - Direction: COUNTERCLOCKWISE
   - Target: 350°
   - Action: Pushes switch LEFT, deactivating heating
2. **Delay:** Wait 500ms for switch to settle
3. **Reposition Phase:** Move clockwise from 350° to 50° (60° movement)
   - Direction: CLOCKWISE
   - Target: 50°
   - Action: Prepares handle on right side for next heat on command

**Postcondition:** Motor is at POSITION_RIGHT (50°), ready for heat on