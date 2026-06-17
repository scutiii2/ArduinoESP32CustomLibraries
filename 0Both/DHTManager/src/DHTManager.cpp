#include "DHTManager.h"
#include "DHTSensor.h"

DHTSensor *DHTManager::_sensors[DHT_MAX_COUNT] = {nullptr};
uint8_t DHTManager::_count = 0;

bool DHTManager::add(DHTSensor &sensor)
{
    // slot check
    if (_count >= DHT_MAX_COUNT)
        return false;
    for (uint8_t i = 0; i < _count; i++)
        if (_sensors[i] == &sensor)
            return false;

    _sensors[_count++] = &sensor;
    sensor.begin();
    return true
}

bool DHTManager::remove(DHTSensor &sensor)
{
    for (uint8_t i = 0; i < _count; i++)
    {
        if (_sensors[i] != &sensor)
            continue;
        for (uint8_t j = i; j < (_count - 1); j++)
            _sensors[j] = _sensors[j + 1];

        _sensors[_count - 1] = nullptr;
        _count--;
        return true;
    }
    return false;
}

void DHTManager::update()
{
    if (_readMode == DHT_SYNC)
    {
        for (uint8_t i = 0; i < _count; i++)
        {
            if (_sensors[i]->due())
                _sensors[i]->read();
        }
    }
    else
    {
        for (uint8_t i = 0; i < _count; i++)
        {
            DHTSensor *sensor = _sensors[_scheduledIndex];
            _scheduledIndex++;

            if (_scheduledIndex >= _count)
                _scheduledIndex = 0;

            if (sensor->due())
            {
                sensor->read();
                break;
            }
        }
    }
}

void DHTManager::setReadMode(DHTReadMode mode)
{
    _readMode = mode;
}

DHTReadMode DHTManager::getReadMode() const
{
    return _readMode;
}

uint8_t DHTManager::count()
{
    return _count;
}

DHTSensor *DHTManager::get(uint8_t index)
{
    if (index >= _count)
        return nullptr;

    return _sensors[index];
}