#ifndef SENSOR_DEFINES_H
#define SENSOR_DEFINES_H

#include "../discovery/discovery.h"   /* sensor_flag_t: which sensor produced a reading */

/* How often each sensor is supposed to report. The discovery task treats a sensor as gone after
 * DISCOVERY_MISSED_READINGS of these pass with no valid reading, so the sensor task's delay and the
 * health struct must use the same numbers. */
#define SHT31_INTERVAL_MS    10000   /* delay between SHT31 readings */
#define DS18B20_INTERVAL_MS   6000   /* full DS18B20 cycle: the 750 ms conversion plus the remaining delay */

/* How often the discovery task runs one discovery_step() pass. */
#define DISCOVERY_PERIOD_MS   5000

typedef enum {
    TEMPERATURE,
    HUMIDITY
} sensorType_t;

typedef struct {
    sensorType_t type;
    sensor_flag_t source;   /* the sensor that produced it; the publisher maps it to a location */
    float data;
} sensorData_t;

#endif // SENSOR_DEFINES_H