#pragma once

#include <Arduino.h>
#include "DHTSensor.h"

#define DHTMANAGER_MAX_SENSORS 16

class DHTManager
{
public:
    DHTManager();

    bool add(DHTSensor& sensor);

    void update();

    void setReadMode(DHTReadMode mode);
    DHTReadMode getReadMode() const;

    uint8_t count() const;

    DHTSensor* get(uint8_t index);

private:
    DHTSensor* _sensors[DHTMANAGER_MAX_SENSORS];
    uint8_t _count;

    DHTReadMode _readMode;
    uint8_t _scheduledIndex;
};