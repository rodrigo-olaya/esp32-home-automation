#include "discovery.h"

 
uint8_t discovery_scan(const discovery_probes_t *probes) {
    
}
// Calls each probe once. Returns a bitmask of the sensors that answered.

uint8_t discovery_newly_found(uint8_t already_running, uint8_t found_now) {

}
// Returns the sensors that need a new task created for them.

uint8_t discovery_lost(uint8_t already_running, uint8_t found_now) {

}
// Returns the sensors that have a task but stopped answering. The discovery
// task stops those tasks, and clears them from its "running" mask.