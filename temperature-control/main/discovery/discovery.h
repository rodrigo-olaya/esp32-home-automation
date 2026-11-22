#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

typedef enum { SENSOR_SHT31 = 1 << 0, SENSOR_DS18B20 = 1 << 1 } sensor_flag_t;
typedef bool (*probe_fn_t)(void);
typedef struct { 
    probe_fn_t sht31; 
    probe_fn_t ds18b20; 
} discovery_probes_t;
 
uint8_t discovery_scan(const discovery_probes_t *probes);
// Calls each probe once. Returns a bitmask of the sensors that answered.

uint8_t discovery_newly_found(uint8_t already_running, uint8_t found_now);
// Returns the sensors that need a new task created for them.

uint8_t discovery_lost(uint8_t already_running, uint8_t found_now);
// Returns the sensors that have a task but stopped answering. The discovery
// task stops those tasks, and clears them from its "running" mask.