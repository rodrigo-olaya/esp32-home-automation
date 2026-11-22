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

    
    return sht31present | (ds18b20present << 1);
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
    
    return sht31NewlyFound | (ds18b20NewlyFound << 1);
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
    
    return sht31Dropped | (ds18b20Dropped << 1);
}