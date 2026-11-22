#ifndef DISCOVERY_H
#define DISCOVERY_H

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

typedef enum {
    SHT31_BIT = 0,
    DS18B20_BIT = 1
} bitmap;

typedef enum { 
    SENSOR_SHT31 = 1 << SHT31_BIT, 
    SENSOR_DS18B20 = 1 << DS18B20_BIT 
} sensor_flag_t;

typedef bool (*probe_fn_t)(void);

typedef struct { 
    probe_fn_t sht31; 
    probe_fn_t ds18b20; 
} discovery_probes_t;

typedef struct {
    uint32_t last_seen_ms;   // tick time of the newest VALID reading from this sensor
    uint32_t interval_ms;    // how often it is supposed to report
} sensor_health_t;

typedef struct { 
    sensor_health_t sht31; 
    sensor_health_t ds18b20; 
} discovery_health_t;

typedef void (*sensor_action_fn_t)(sensor_flag_t sensor);
typedef struct { sensor_action_fn_t start; sensor_action_fn_t stop; } discovery_actions_t;
typedef struct { uint8_t running; } discovery_state_t;     // mask of sensors that have a task

#define DISCOVERY_MISSED_READINGS 3
 
uint8_t discovery_scan(const discovery_probes_t *probes);
// Calls each probe once. Returns a bitmask of the sensors that answered.

uint8_t discovery_newly_found(uint8_t already_running, uint8_t found_now);
// Returns the sensors that need a new task created for them.

uint8_t discovery_lost(uint8_t already_running, uint8_t found_now);
// Returns the sensors that have a task but stopped answering. The discovery
// task stops those tasks, and clears them from its "running" mask.

uint8_t discovery_recently_seen(const discovery_health_t *health, uint32_t now_ms);
    // Returns a bitmask of the sensors that reported recently enough. Feed it to
    // discovery_lost() as found_now for sensors that already have a task.

void discovery_step(discovery_state_t *state,
                    const discovery_probes_t *probes,
                    discovery_health_t *health,
                    uint32_t now_ms,
                    const discovery_actions_t *actions);

#endif /* DISCOVERY_H */