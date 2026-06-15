#include "DHTManager.h"

DHTManager::DHTManager()
    : _count(0),
      _readMode(DHT_SYNC),
      _scheduledIndex(0)
{
}

bool DHTManager::add(DHTSensor& sensor)
{
    if (_count >= DHTMANAGER_MAX_SENSORS)
        return false;

    _sensors[_count++] = &sensor;

    sensor.begin();

    return true;
}

void DHTManager::update()
{
    if (!_count)
        return;

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
            DHTSensor* sensor = _sensors[_scheduledIndex];

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

uint8_t DHTManager::count() const
{
    return _count;
}

DHTSensor* DHTManager::get(uint8_t index)
{
    if (index >= _count)
        return nullptr;

    return _sensors[index];
}