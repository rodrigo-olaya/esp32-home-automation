# 1-Wire Protocol Requirements

## 1. System Overview

### Purpose
Implement a generic 1-Wire protocol driver for communication with 1-Wire devices (e.g., DS18B20, future devices) on the ESP32 platform.

### Hardware
- **Protocol:** Dallas/Maxim 1-Wire protocol
- **Interface:** Single GPIO pin on ESP32
- **Power:** Parasite power or external 3.3V/5V supply

### Integration
- Provide device discovery, bus management, and low-level read/write operations for all 1-Wire devices
- Device-specific drivers (e.g., DS18B20) will use this protocol layer

---

## 2. Hardware Configuration

### GPIO Assignment
- **Data Pin:** GPIO_NUMBER_X (to be defined in `gpio_config.h`)
- **Pull-up Resistor:** 4.7kΩ required between data line and VCC
- **Power Mode:** Normal power (3.3V) recommended over parasite power for stability

### Wiring Specifications
```
1-Wire Bus Pin Configuration:
- Data (DQ)     → ESP32 GPIO_X + 4.7kΩ pull-up to 3.3V
- GND           → ESP32 GND
- VDD (if used) → ESP32 3.3V (or 5V if using level shifter)
```

### Multi-Device Support
- 1-Wire bus supports multiple devices on the same data line
- Each device has a unique 64-bit ROM address
- System should support 1-5 devices initially (scalable design)

---

## 3. Functional Requirements

### 3.1 Initialization

**REQ-1W-INIT-01:** GPIO Initialization
- Configure 1-Wire GPIO pin as open-drain with pull-up
- Initialize 1-Wire bus using esp-idf-lib onewire driver or custom implementation
- Verify bus communication with presence pulse detection

**REQ-1W-INIT-02:** Device Discovery
- Perform ROM search to detect all devices on bus
- Store 64-bit ROM addresses of discovered devices
- Log number of devices found and their addresses
- Return error if no devices detected

**REQ-1W-INIT-03:** Device Validation
- Check CRC of ROM addresses
- Reject invalid or corrupted device addresses

### 3.2 Bus Operations

**REQ-1W-BUS-01:** Reset and Presence
- Implement bus reset and presence detection per 1-Wire spec
- Detect no presence pulse (no device connected)

**REQ-1W-BUS-02:** Read/Write Slots
- Implement bit and byte-level read/write operations
- Follow 1-Wire timing requirements for all slots

**REQ-1W-BUS-03:** ROM Commands
- Support ROM commands: SEARCH_ROM, READ_ROM, MATCH_ROM, SKIP_ROM
- Allow device-specific drivers to use these commands

### 3.3 Error Handling

**REQ-1W-ERR-01:** Communication Errors
- Detect and handle:
  - No presence pulse (device disconnected)
  - CRC mismatch (corrupted data)
  - Bus short circuit or open circuit
  - Timeout during operations
- Log specific error type with ESP_LOGE

**REQ-1W-ERR-02:** Recovery Mechanism
- Retry failed operations up to 3 times with delay
- Re-initialize bus if persistent failures detected
- Continue operation with remaining devices if one device fails

---

## 4. Task Architecture

### 4.1 Task Design

- Provide thread-safe access to the 1-Wire bus
- Use FreeRTOS mutex or critical sections for bus operations
- Only one master operation at a time (half-duplex)

---

## 5. Performance Requirements

**REQ-1W-PERF-01:** Timing Accuracy
- Follow Dallas 1-Wire protocol timing specifications exactly:
  - Write 0 slot: 60-120 µs
  - Write 1 slot: 1-15 µs low, then release
  - Read slot: 1-15 µs low, then sample within 15 µs
  - Reset pulse: 480-960 µs
  - Presence pulse: 60-240 µs after reset
- Use esp-idf-lib implementation or ensure custom code meets timing

**REQ-1W-PERF-02:** CPU Usage
- Minimize CPU blocking during operations
- Use non-blocking delays where possible

**REQ-1W-PERF-03:** Memory Usage
- Device ROM addresses: 8 bytes per device
- Total heap usage: <1KB for 5 devices

---

## 6. Logging and Debugging

**REQ-1W-LOG-01:** Initialization Logging
```
ESP_LOGI(TAG, "Initializing 1-Wire bus on GPIO %d", GPIO_PIN);
ESP_LOGI(TAG, "Found %d device(s)", device_count);
ESP_LOGI(TAG, "Device %d ROM: %016llX", index, rom_address);
```

**REQ-1W-LOG-02:** Runtime Logging
```
ESP_LOGE(TAG, "CRC error reading device %d", device_index);
ESP_LOGW(TAG, "No presence pulse detected");
```

**REQ-1W-LOG-03:** Debug Level Control
- Use ESP_LOG_DEBUG for detailed protocol debugging
- Use ESP_LOG_INFO for normal operation events
- Use ESP_LOG_ERROR for failures requiring attention

---

## 7. Configuration

### 7.1 Compile-Time Configuration

Define in `gpio_config.h`:
```c
#define ONE_WIRE_GPIO GPIO_NUMBER_X        // GPIO pin for 1-wire bus
```

Define in configuration file:
```c
#define MAX_ONE_WIRE_DEVICES 5             // Maximum supported devices
#define ONE_WIRE_OP_RETRIES 3              // Number of retry attempts
```

### 7.2 Runtime Configuration (Future)
- Consider adding runtime configuration for bus timing, device enable/disable, etc.

---

## 8. Testing Requirements

**REQ-1W-TEST-01:** Unit Tests
- CRC validation functions
- ROM search and device discovery
- Error detection and recovery

**REQ-1W-TEST-02:** Integration Tests
- Multi-device discovery and communication
- Error recovery scenarios

**REQ-1W-TEST-03:** Hardware Tests
- Test with device disconnection/reconnection
- Verify multi-device operation

---

## 9. Dependencies

### 9.1 External Libraries
- `esp-idf-lib/onewire`: Low-level 1-wire protocol driver (optional)

### 9.2 Internal Dependencies
- `drivers/gpio_driver.h`: GPIO abstraction layer
- FreeRTOS: Task creation, delays, queue operations

### 9.3 Component Configuration
Already defined in `main/idf_component.yml`:
```yaml
esp-idf-lib/onewire: '*'
```

---

## 10. Implementation Notes

### 10.1 Code Organization
```
main/sensors/
├── onewire.h           // Public API and configuration
├── onewire.c           // Implementation
```

### 10.2 API Design Example
```c
// Initialization
esp_err_t onewire_init(gpio_number_t gpio_pin);

// Device discovery
uint8_t onewire_search(uint64_t *roms, uint8_t max_devices);

// Low-level operations
esp_err_t onewire_reset(void);

uint8_t onewire_read_byte(void);
```

### 10.3 Integration Points
1. Call `onewire_init()` in `app_main()` before starting device-specific drivers
2. Device-specific drivers (e.g., DS18B20) use this API for bus operations

---

## 11. Future Enhancements

**Enhancement Ideas:**
- Add support for additional 1-Wire devices (e.g., EEPROM, switches)
- Support hot-swap device detection (dynamic ROM search)
- Add bus diagnostics and statistics

---

## 12. Success Criteria

**The 1-Wire protocol implementation is complete when:**
1. ✅ Devices are discovered and initialized on boot
2. ✅ Device ROM addresses are validated (CRC)
3. ✅ Device-specific drivers can communicate via the bus
4. ✅ CRC errors and bus errors are detected and handled
5. ✅ Multi-device operation works correctly
6. ✅ Code passes unit tests for bus operations
7. ✅ Device disconnection/reconnection is handled gracefully
**REQ-1W-TEST-01:** Unit Tests
