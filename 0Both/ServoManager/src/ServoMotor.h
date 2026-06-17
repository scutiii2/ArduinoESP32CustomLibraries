#pragma once

#include <optional>
#include <Arduino.h>
#include <ESP32Servo.h>

#define SERVO_DEFAULT_MIN_PULSE 500
#define SERVO_DEFAULT_CENTER_PULSE 1500
#define SERVO_DEFAULT_MAX_PULSE 2500
#define SERVO_DEFAULT_UPDATE_INTERVAL 20
enum ServoMoveMode
{
    SERVO_IMMEDIATE,
    SERVO_SMOOTH
};

class ServoMotor
{
public:
    // MAIN =============
    ServoMotor(
        uint8_t pin,
        int minPulse = SERVO_DEFAULT_MIN_PULSE,
        int maxPulse = SERVO_DEFAULT_MAX_PULSE,
        std::optional<int> centerPulse = std::nullopt);
    ~ServoMotor();
    void update();

    // PROCESSORS =============
    void calibrate(
        int minPulse = SERVO_DEFAULT_MIN_PULSE,
        int maxPulse = SERVO_DEFAULT_MAX_PULSE,
        std::optional<int> centerPulse = std::nullopt);
    void writeMicroseconds(int pulse);
    void write(int angle);
    void moveTo(
        int angle,
        uint32_t duration);
    void moveBy(
        int deltaAngle,
        uint32_t duration);

    // GETTERS | SETTERS =============
    void setUpdateInterval(uint32_t intervalMs);
    void center();
    int readAngle() const;
    int readPulse() const;
    int getMinPulse() const;
    int getCenterPulse() const;
    int getMaxPulse() const;
    uint8_t getPin() const;
    bool isMoving() const;

private:
    int angleToPulse(int angle) const;
    int pulseToAngle(int pulse) const;

    Servo _servo;

    uint8_t _pin;
    int _minPulse;
    int _centerPulse;
    int _maxPulse;

    int _currentAngle;
    int _currentPulse;
    int _startAngle;
    int _targetAngle;

    uint32_t _moveDuration;
    uint32_t _moveStartTime;
    uint32_t _updateInterval;
    uint32_t _lastUpdateTime;
    bool _moving;
};