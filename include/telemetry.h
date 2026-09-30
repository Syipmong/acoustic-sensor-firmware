#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

struct TelemetryPayload
{
    uint16_t node_id;
    uint32_t epoch_time;
    uint16_t sequence_num;
    float   latitude;
    float   longitude;
    uint8_t threat_class;
    uint8_t confidence;
    float   battery_volts;
    uint8_t signature[64];
} __attribute__((packed));

#endif /* TELEMETRY_H */