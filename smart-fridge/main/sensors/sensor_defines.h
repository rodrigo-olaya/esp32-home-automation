#ifndef SENSOR_DEFINES_H
#define SENSOR_DEFINES_H

typedef enum {
    TEMPERATURE,
    HUMIDITY
} sensorType_t;

typedef struct {
    sensorType_t type;
    float data;
} sensorData_t;

#endif // SENSOR_DEFINES_H