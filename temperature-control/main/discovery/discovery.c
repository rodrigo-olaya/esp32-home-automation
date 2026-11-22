#include "discovery.h"

uint8_t discovery_scan(const discovery_probes_t *probes) {
    bool sht31present = false;
    bool ds18b20present = false;
    if (probes->sht31 != NULL) {
        sht31present = probes->sht31();
    }
    if (probes->ds18b20 != NULL) {
        ds18b20present = probes->ds18b20();
    }
    
    return (sht31present << SHT31_BIT) | (ds18b20present << DS18B20_BIT);
}

uint8_t discovery_newly_found(uint8_t already_running, uint8_t found_now) {
    uint8_t sht31NewlyFound = 0;
    uint8_t ds18b20NewlyFound = 0;
    if (((already_running & SENSOR_SHT31) != SENSOR_SHT31) && ((found_now & SENSOR_SHT31) == SENSOR_SHT31)) {
        sht31NewlyFound = 1;
    }
    if (((already_running & SENSOR_DS18B20) != SENSOR_DS18B20) && ((found_now & SENSOR_DS18B20) == SENSOR_DS18B20)) {
        ds18b20NewlyFound = 1;
    }
    
    return (sht31NewlyFound << SHT31_BIT) | (ds18b20NewlyFound << DS18B20_BIT);
}

uint8_t discovery_lost(uint8_t already_running, uint8_t found_now) {
    uint8_t sht31Dropped = 0;
    uint8_t ds18b20Dropped = 0;
    if (((already_running & SENSOR_SHT31) == SENSOR_SHT31) && ((found_now & SENSOR_SHT31) != SENSOR_SHT31)) {
        sht31Dropped = 1;
    }
    if (((already_running & SENSOR_DS18B20) == SENSOR_DS18B20) && ((found_now & SENSOR_DS18B20) != SENSOR_DS18B20)) {
        ds18b20Dropped = 1;
    }
    
    return (sht31Dropped << SHT31_BIT) | (ds18b20Dropped << DS18B20_BIT);
}

uint8_t discovery_recently_seen(const discovery_health_t *health, uint32_t now_ms) {
    uint8_t sht31Healthy = 0;
    uint8_t ds18b20Healthy = 0;
    
    if (now_ms - health->sht31.last_seen_ms <= (health->sht31.interval_ms * 3)) {
        sht31Healthy = 1;
    }
    if (now_ms - health->ds18b20.last_seen_ms <= (health->ds18b20.interval_ms * 3)) {
        ds18b20Healthy = 1;
    }
 
    return (sht31Healthy << SHT31_BIT) | (ds18b20Healthy << DS18B20_BIT);
}

void discovery_step(discovery_state_t *state,
    const discovery_probes_t *probes,
    discovery_health_t *health,
    uint32_t now_ms,
    const discovery_actions_t *actions) {
        uint8_t sht31Running = state->running & SENSOR_SHT31;
        uint8_t ds18b20Running = state->running & SENSOR_DS18B20;

        discovery_probes_t nonRunningProbes;
        if (sht31Running == 1) {
            nonRunningProbes.sht31 = NULL;
        }
        else {
            nonRunningProbes.sht31 = probes->sht31;
        }
        if (ds18b20Running == 1) {
            nonRunningProbes.ds18b20 = NULL;
        }
        else {
            nonRunningProbes.ds18b20 = probes->ds18b20;
        }

        uint8_t discoveryBitmask = discovery_scan(&nonRunningProbes);
        uint8_t newlyFound = discovery_newly_found(state->running, discoveryBitmask);

        if ((newlyFound & SENSOR_SHT31) == SENSOR_SHT31){
            actions->start(SENSOR_SHT31);
            health->sht31.last_seen_ms = now_ms;
            state->running |= SENSOR_SHT31;
        }
        if ((newlyFound & SENSOR_DS18B20) == SENSOR_DS18B20){
            actions->start(SENSOR_DS18B20);
            health->ds18b20.last_seen_ms = now_ms;
            state->running |= SENSOR_DS18B20;
        }
        
        uint8_t recentlySeen = discovery_recently_seen(health, now_ms);

        if ((recentlySeen & SENSOR_SHT31) != SENSOR_SHT31){
            actions->stop(SENSOR_SHT31);
            state->running &= ~(SENSOR_SHT31); 
        }
        if ((recentlySeen & SENSOR_DS18B20) != SENSOR_DS18B20){
            actions->stop(SENSOR_DS18B20);
            state->running &= ~(SENSOR_DS18B20); 
        }
}