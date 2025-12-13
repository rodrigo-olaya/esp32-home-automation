# 1-Wire Protocol Requirements

## Purpose
Implement a minimal, hardware-agnostic 1-Wire protocol driver in C for the ESP32 (or similar MCU), with no dependencies on Espressif or vendor libraries. The driver must support device discovery, bus reset, and bit/byte-level read/write for any 1-Wire device.

## Electrical
- 1-Wire bus: single GPIO, open-drain, 4.7kΩ pull-up to 3.3V or 5V.
- Multiple devices supported (each has unique 64-bit ROM).

## Protocol Description
The 1-Wire protocol is a single-wire, half-duplex, time-critical protocol. All communication is initiated by the master (MCU). Key operations:

### 1. Reset & Presence
- Master pulls bus low for 480–960μs (reset pulse).
- Releases bus; waits 15–60μs, then samples for presence pulse.
- Device(s) pull bus low for 60–240μs to signal presence.

### 2. Write Bit
- Write 1: Pull bus low for 1–15μs, then release for rest of slot (total slot: 60μs min).
- Write 0: Pull bus low for 60μs, then release.

### 3. Read Bit
- Master pulls bus low for 1–15μs, then releases.
- Sample bus 15μs after start of slot; device drives bus low for 0 (else leaves high).

### 4. Byte Operations
- Write/read bytes LSB first, 8 bits per byte, using above bit timing.

### 5. ROM Commands
- SEARCH_ROM, READ_ROM, MATCH_ROM, SKIP_ROM, etc. (see Maxim/Dallas datasheets).


## API
```c
void onewire_init(int gpio_pin);           // Set up GPIO for 1-Wire bus
int onewire_reset(void);                   // Send reset, return 1 if device present, 0 if not
void onewire_write_bit(int bit);           // Write a single bit to the bus
int onewire_read_bit(void);                // Read a single bit from the bus
void onewire_write_byte(uint8_t byte);     // Write a byte (LSB first) to the bus
uint8_t onewire_read_byte(void);           // Read a byte (LSB first) from the bus
int onewire_search(uint64_t *roms, int max_devices); // Find ROM codes of all devices (optional)
```

## Implementation Notes
- All timing must be implemented with busy-wait/delay loops or hardware timers (no RTOS or vendor delay functions).
- GPIO must be set to open-drain (or emulate with input/output switching).
- No dynamic memory allocation required.
- No Espressif/IDF/Arduino/ code or dependencies.

## References
- Maxim Integrated Application Note 126: "1-Wire Communication Through Software"
- DS18B20 datasheet, section "1-Wire Bus System"
