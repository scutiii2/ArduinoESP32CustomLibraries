#include "DHTSensor.h"
#include <math.h>

DHTSensor::DHTSensor(uint8_t pin, DHTType type)
    : _pin(pin),
      _type(type),
      _dht(
          pin,
          type == DHT_TYPE_11
              ? DHT11
              : DHT22),
      _lastRead(0),
      _updated(false),
      _valid(false),
      _online(false),
      _successCount(0),
      _failCount(0),
      _consecutiveFails(0),
      _temperature(NAN),
      _humidity(NAN),
      _heatIndex(NAN),
      _dewPoint(NAN),
      _tempOffset(0),
      _humidityOffset(0),
      _comfort(DHT_COMFORT_UNKNOWN),
      _medianSamples(0),
      _averageSamples(0),
      _tempHistoryCount(0),
      _humidityHistoryCount(0),
      _updateCallback(nullptr),
      _errorCallback(nullptr),
      _onlineCallback(nullptr),
      _offlineCallback(nullptr),
      _tempAboveThreshold(NAN),
      _tempBelowThreshold(NAN),
      _humidityAboveThreshold(NAN),
      _humidityBelowThreshold(NAN),
      _tempAboveCallback(nullptr),
      _tempBelowCallback(nullptr),
      _humidityAboveCallback(nullptr),
      _humidityBelowCallback(nullptr),
      _interval(_type == DHT_TYPE_11
                    ? DHT11_DEFAULT_INTERVAL
                    : DHT22_DEFAULT_INTERVAL)
{
    memset(
        _tempHistory,
        0,
        sizeof(_tempHistory));
    memset(
        _humidityHistory,
        0,
        sizeof(_humidityHistory));
}

bool DHTSensor::begin()
{
    _dht.begin();
    return true;
}

bool DHTSensor::read()
{
    float h = _dht.readHumidity();
    float t = _dht.readTemperature();

    _updated = false;

    if (isnan(h) || isnan(t))
    {
        _failCount++;
        _consecutiveFails++;

        if (_errorCallback)
            _errorCallback(*this);

        if (_online && _consecutiveFails >= DHT_OFFLINE_THRESHOLD)
        {
            _online = false;

            if (_offlineCallback)
                _offlineCallback(*this);
        }

        return false;
    }

    bool wasOffline = !_online;

    _successCount++;
    _consecutiveFails = 0;

    _valid = true;
    _online = true;
    _updated = true;

    t += _tempOffset;
    h += _humidityOffset;

    _temperature = applyFiltering(
        t,
        _tempHistory,
        _tempHistoryCount);

    _humidity = applyFiltering(
        h,
        _humidityHistory,
        _humidityHistoryCount);

    updateEnvironment();
    updateComfort();
    updateThresholds();

    if (wasOffline && _onlineCallback)
        _onlineCallback(*this);

    if (_updateCallback)
        _updateCallback(*this);

    _lastRead = millis();

    return true;
}

void DHTSensor::update()
{
    if (due())
        read();
}

bool DHTSensor::due() const
{
    return millis() - _lastRead >= _interval;
}

bool DHTSensor::updated()
{
    bool value = _updated;
    _updated = false;
    return value;
}

bool DHTSensor::valid() const
{
    return _valid;
}

bool DHTSensor::online() const
{
    return _online;
}

uint8_t DHTSensor::pin() const
{
    return _pin;
}

DHTType DHTSensor::type() const
{
    return _type;
}

float DHTSensor::temperature() const
{
    return _temperature;
}

float DHTSensor::humidity() const
{
    return _humidity;
}

float DHTSensor::heatIndex() const
{
    return _heatIndex;
}

float DHTSensor::dewPoint() const
{
    return _dewPoint;
}

DHTComfort DHTSensor::comfort() const
{
    return _comfort;
}

void DHTSensor::setInterval(uint32_t interval)
{
    _interval = interval;
}

uint32_t DHTSensor::interval() const
{
    return _interval;
}

void DHTSensor::setTempOffset(float offset)
{
    _tempOffset = offset;
}

void DHTSensor::setHumidityOffset(float offset)
{
    _humidityOffset = offset;
}

float DHTSensor::tempOffset() const
{
    return _tempOffset;
}

float DHTSensor::humidityOffset() const
{
    return _humidityOffset;
}

void DHTSensor::setMedianSamples(uint8_t samples)
{
    _medianSamples = min(samples, (uint8_t)DHT_MAX_FILTER_SAMPLES);
}

void DHTSensor::setAverageSamples(uint8_t samples)
{
    _averageSamples = min(samples, (uint8_t)DHT_MAX_FILTER_SAMPLES);
}

uint8_t DHTSensor::medianSamples() const
{
    return _medianSamples;
}

uint8_t DHTSensor::averageSamples() const
{
    return _averageSamples;
}

uint32_t DHTSensor::successCount() const
{
    return _successCount;
}

uint32_t DHTSensor::failCount() const
{
    return _failCount;
}

float DHTSensor::successRate() const
{
    uint32_t total = _successCount + _failCount;

    if (!total)
        return 0;

    return (_successCount * 100.0f) / total;
}

void DHTSensor::updateEnvironment()
{
    _heatIndex = _dht.computeHeatIndex(
        _temperature,
        _humidity,
        false);

    double a = 17.27;
    double b = 237.7;

    double alpha =
        ((a * _temperature) / (b + _temperature)) +
        log(_humidity / 100.0);

    _dewPoint =
        (b * alpha) /
        (a - alpha);
}

void DHTSensor::updateComfort()
{
    if (_humidity < 30)
        _comfort = DHT_COMFORT_DRY;
    else if (_humidity <= 60)
        _comfort = DHT_COMFORT_COMFORTABLE;
    else
        _comfort = DHT_COMFORT_HUMID;
}

void DHTSensor::updateThresholds()
{
    if (_tempAboveCallback &&
        !isnan(_tempAboveThreshold) &&
        _temperature > _tempAboveThreshold)
        _tempAboveCallback(*this);

    if (_tempBelowCallback &&
        !isnan(_tempBelowThreshold) &&
        _temperature < _tempBelowThreshold)
        _tempBelowCallback(*this);

    if (_humidityAboveCallback &&
        !isnan(_humidityAboveThreshold) &&
        _humidity > _humidityAboveThreshold)
        _humidityAboveCallback(*this);

    if (_humidityBelowCallback &&
        !isnan(_humidityBelowThreshold) &&
        _humidity < _humidityBelowThreshold)
        _humidityBelowCallback(*this);
}

float DHTSensor::applyFiltering(
    float value,
    float *history,
    uint8_t &historyCount)
{
    for (int i = DHT_MAX_FILTER_SAMPLES - 1; i > 0; i--)
        history[i] = history[i - 1];

    history[0] = value;

    if (historyCount < DHT_MAX_FILTER_SAMPLES)
        historyCount++;

    uint8_t count = historyCount;

    float result = value;

    if (_medianSamples > 0)
    {
        result = median(
            history,
            min(count, _medianSamples));
    }

    if (_averageSamples > 0)
    {
        result = average(
            history,
            min(count, _averageSamples));
    }

    return result;
}

float DHTSensor::median(float *values, uint8_t count)
{
    float buffer[DHT_MAX_FILTER_SAMPLES];

    memcpy(buffer, values, count * sizeof(float));

    for (uint8_t i = 0; i < count - 1; i++)
    {
        for (uint8_t j = i + 1; j < count; j++)
        {
            if (buffer[j] < buffer[i])
            {
                float temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
            }
        }
    }

    if (count & 1)
        return buffer[count / 2];

    return (
               buffer[(count / 2) - 1] +
               buffer[count / 2]) *
           0.5f;
}

float DHTSensor::average(float *values, uint8_t count)
{
    float sum = 0;

    for (uint8_t i = 0; i < count; i++)
        sum += values[i];

    return sum / count;
}

void DHTSensor::onUpdate(Callback callback) { _updateCallback = callback; }
void DHTSensor::onError(Callback callback) { _errorCallback = callback; }
void DHTSensor::onOnline(Callback callback) { _onlineCallback = callback; }
void DHTSensor::onOffline(Callback callback) { _offlineCallback = callback; }

void DHTSensor::onTemperatureAbove(float value, Callback callback)
{
    _tempAboveThreshold = value;
    _tempAboveCallback = callback;
}

void DHTSensor::onTemperatureBelow(float value, Callback callback)
{
    _tempBelowThreshold = value;
    _tempBelowCallback = callback;
}

void DHTSensor::onHumidityAbove(float value, Callback callback)
{
    _humidityAboveThreshold = value;
    _humidityAboveCallback = callback;
}

void DHTSensor::onHumidityBelow(float value, Callback callback)
{
    _humidityBelowThreshold = value;
    _humidityBelowCallback = callback;
}