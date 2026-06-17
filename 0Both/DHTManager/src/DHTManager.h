#pragma once

#include <Arduino.h>
#include "DHTSensor.h"

#define DHT_MAX_COUNT 16

class DHTManager
{
public:
    static bool add(DHTSensor& sensor);
    static bool remove(DHTSensor& servo);
    static void update();

    static void setReadMode(DHTReadMode mode);
    static DHTReadMode getReadMode() const;
    static uint8_t count();
    static DHTSensor* get(uint8_t index);

private:
    static DHTSensor* _sensors[DHT_MAX_COUNT];
    static uint8_t _count;

    static DHTReadMode _readMode;
    static uint8_t _scheduledIndex;
};