#pragma once

#include <Arduino.h>
#include <DHT.h>

#define DHTMANAGER_MAX_FILTER_SAMPLES 10
#define DHT11_DEFAULT_INTERVAL 1000UL
#define DHT22_DEFAULT_INTERVAL 2000UL
#define DHT_OFFLINE_THRESHOLD 5

enum DHTType
{
    DHT_TYPE_11,
    DHT_TYPE_22
};

enum DHTReadMode
{
    DHT_SYNC,
    DHT_SCHEDULED
};

enum DHTComfort
{
    DHT_COMFORT_UNKNOWN,
    DHT_COMFORT_DRY,
    DHT_COMFORT_COMFORTABLE,
    DHT_COMFORT_HUMID
};

class DHTSensor
{
public:
    typedef void (*Callback)(DHTSensor &);

    DHTSensor(uint8_t pin, DHTType type);

    bool begin();

    bool read();
    void update();

    bool due() const;

    bool updated();
    bool valid() const;
    bool online() const;

    uint8_t pin() const;
    DHTType type() const;

    float temperature() const;
    float humidity() const;

    float heatIndex() const;
    float dewPoint() const;

    DHTComfort comfort() const;

    void setInterval(uint32_t interval);
    uint32_t interval() const;

    void setTempOffset(float offset);
    void setHumidityOffset(float offset);

    float tempOffset() const;
    float humidityOffset() const;

    void setMedianSamples(uint8_t samples);
    void setAverageSamples(uint8_t samples);

    uint8_t medianSamples() const;
    uint8_t averageSamples() const;

    uint32_t successCount() const;
    uint32_t failCount() const;

    float successRate() const;

    void onUpdate(Callback callback);
    void onError(Callback callback);
    void onOnline(Callback callback);
    void onOffline(Callback callback);

    void onTemperatureAbove(float value, Callback callback);
    void onTemperatureBelow(float value, Callback callback);
    void onHumidityAbove(float value, Callback callback);
    void onHumidityBelow(float value, Callback callback);

private:
    uint8_t _pin;
    DHTType _type;

    DHT _dht;

    uint32_t _interval;
    uint32_t _lastRead;

    bool _updated;
    bool _valid;
    bool _online;

    uint32_t _successCount;
    uint32_t _failCount;
    uint8_t _consecutiveFails;

    float _temperature;
    float _humidity;

    float _heatIndex;
    float _dewPoint;

    float _tempOffset;
    float _humidityOffset;

    DHTComfort _comfort;

    uint8_t _medianSamples;
    uint8_t _averageSamples;

    float _tempHistory[DHTMANAGER_MAX_FILTER_SAMPLES];
    float _humidityHistory[DHTMANAGER_MAX_FILTER_SAMPLES];
    uint8_t _tempHistoryCount;
    uint8_t _humidityHistoryCount;

    Callback _updateCallback;
    Callback _errorCallback;
    Callback _onlineCallback;
    Callback _offlineCallback;

    float _tempAboveThreshold;
    float _tempBelowThreshold;
    float _humidityAboveThreshold;
    float _humidityBelowThreshold;

    Callback _tempAboveCallback;
    Callback _tempBelowCallback;
    Callback _humidityAboveCallback;
    Callback _humidityBelowCallback;

    void updateComfort();
    void updateEnvironment();
    void updateThresholds();

    float applyFiltering(
        float value,
        float *history,
        uint8_t &historyCount);
    float median(float *values, uint8_t count);
    float average(float *values, uint8_t count);
};