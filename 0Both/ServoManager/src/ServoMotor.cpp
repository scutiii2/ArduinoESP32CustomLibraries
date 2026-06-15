#include "ServoMotor.h"
#include <math.h>

ServoMotor::ServoMotor()
{
    _pin = 255;
    _minPulse = SERVO_DEFAULT_MIN_PULSE;
    _centerPulse = SERVO_DEFAULT_CENTER_PULSE;
    _maxPulse = SERVO_DEFAULT_MAX_PULSE;

    _currentAngle = 90;
    _currentPulse = _centerPulse;

    _startAngle = 90;
    _targetAngle = 90;

    _moveDuration = 0;
    _moveStartTime = 0;

    _updateInterval = SERVO_DEFAULT_UPDATE_INTERVAL;
    _lastUpdateTime = 0;

    _attached = false;
    _moving = false;
}

ServoMotor::ServoMotor(uint8_t pin)
    : ServoMotor()
{
    attach(pin);
}

ServoMotor::ServoMotor(
    uint8_t pin,
    int minPulse,
    int maxPulse)
    : ServoMotor()
{
    attach(pin, minPulse, maxPulse);
}

ServoMotor::~ServoMotor()
{
    detach();
}

void ServoMotor::detach()
{
    if (!_attached)
        return;

    _servo.detach();
    _attached = false;
    _moving = false;
}

bool ServoMotor::attach(
    uint8_t pin,
    int minPulse,
    int maxPulse)
{
    calibrate(minPulse, maxPulse, ) return attach(pin);
}

bool ServoMotor::attach(uint8_t pin)
{
    if (_attached)
        detach();

    _pin = pin;
    _servo.setPeriodHertz(50);
    _servo.attach(_pin, _minPulse, _maxPulse);
    _attached = true;
    write(_currentAngle);
    return true;
}

bool ServoMotor::attached() const
{
    return _attached;
}

void ServoMotor::calibrate(
    int minPulse,
    int maxPulse,
    int centerPulse)
{
    if (maxPulse <= minPulse)
        return;
    _minPulse = minPulse;
    _maxPulse = maxPulse;
    _centerPulse = (centerPulse <= minPulse || centerPulse >= maxPulse) ? ((minPulse + maxPulse) / 2) : centerPulse;
}

void ServoMotor::calibrate(
    int minPulse,
    int maxPulse)
{
    calibrate(minPulse, maxPulse, (minPulse + maxPulse) / 2);
}

void ServoMotor::setUpdateInterval(uint32_t intervalMs)
{
    _updateInterval = intervalMs;
}

void ServoMotor::write(int angle)
{
    angle = constrain(angle, 0, 180);

    _currentAngle = angle;
    _currentPulse = angleToPulse(angle);

    _moving = false;

    if (_attached)
        _servo.writeMicroseconds(_currentPulse);
}

void ServoMotor::writeMicroseconds(int pulse)
{
    pulse = constrain(
        pulse,
        _minPulse,
        _maxPulse);

    _currentPulse = pulse;
    _currentAngle = pulseToAngle(pulse);

    _moving = false;

    if (_attached)
        _servo.writeMicroseconds(_currentPulse);
}

void ServoMotor::moveTo(
    int angle,
    uint32_t duration)
{
    angle = constrain(angle, 0, 180);

    if (duration == 0)
    {
        write(angle);
        return;
    }

    _startAngle = _currentAngle;
    _targetAngle = angle;

    _moveDuration = duration;
    _moveStartTime = millis();

    _moving = true;
}

void ServoMotor::moveBy(
    int deltaAngle,
    uint32_t duration)
{
    moveTo(
        _currentAngle + deltaAngle,
        duration);
}

void ServoMotor::center()
{
    writeMicroseconds(_centerPulse);
}

int ServoMotor::read() const
{
    return _currentAngle;
}

int ServoMotor::readMicroseconds() const
{
    return _currentPulse;
}

int ServoMotor::getMinPulse() const
{
    return _minPulse;
}

int ServoMotor::getCenterPulse() const
{
    return _centerPulse;
}

int ServoMotor::getMaxPulse() const
{
    return _maxPulse;
}

uint8_t ServoMotor::getPin() const
{
    return _pin;
}

bool ServoMotor::isMoving() const
{
    return _moving;
}

void ServoMotor::update()
{
    if (!_attached)
    {
        return;
    }

    if (!_moving)
    {
        return;
    }

    uint32_t now = millis();

    if (
        _updateInterval > 0 &&
        now - _lastUpdateTime < _updateInterval)
    {
        return;
    }

    _lastUpdateTime = now;

    uint32_t elapsed = now - _moveStartTime;

    if (elapsed >= _moveDuration)
    {
        write(_targetAngle);
        return;
    }

    float progress =
        (float)elapsed /
        (float)_moveDuration;

    int angle =
        _startAngle +
        ((_targetAngle - _startAngle) * progress);

    angle = constrain(angle, 0, 180);

    _currentAngle = angle;
    _currentPulse = angleToPulse(angle);

    _servo.writeMicroseconds(_currentPulse);
}

int ServoMotor::angleToPulse(
    int angle) const
{
    angle = constrain(angle, 0, 180);

    if (angle <= 90)
    {
        return map(
            angle,
            0,
            90,
            _minPulse,
            _centerPulse);
    }

    return map(
        angle,
        90,
        180,
        _centerPulse,
        _maxPulse);
}

int ServoMotor::pulseToAngle(
    int pulse) const
{
    pulse = constrain(
        pulse,
        _minPulse,
        _maxPulse);

    if (pulse <= _centerPulse)
    {
        return map(
            pulse,
            _minPulse,
            _centerPulse,
            0,
            90);
    }

    return map(
        pulse,
        _centerPulse,
        _maxPulse,
        90,
        180);
}