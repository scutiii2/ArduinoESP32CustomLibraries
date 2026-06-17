#include "ServoManager.h"
#include "ServoMotor.h"
#include <math.h>

// ============= MAIN ====================================================
ServoMotor::ServoMotor(uint8_t pin, int minPulse, int maxPulse, std::optional<int> centerPulse)
    : _pin(pin),
      _currentAngle(90),
      _startAngle(90),
      _targetAngle(90),
      _moveDuration(0),
      _moveStartTime(0),
      _updateInterval(SERVO_DEFAULT_UPDATE_INTERVAL),
      _lastUpdateTime(0),
      _moving(false)
{
    calibrate(minPulse, maxPulse, centerPulse);
    _currentPulse = _centerPulse;
    _servo.setPeriodHertz(50);
    _servo.attach(_pin, _minPulse, _maxPulse);
    write(_currentAngle);
    ServoManager::add(*this);
}

ServoMotor::~ServoMotor()
{
    ServoManager::remove(*this);
}

void ServoMotor::update()
{
    if (!_moving)
        return;

    uint32_t now = millis();
    if (
        _updateInterval > 0 &&
        now - _lastUpdateTime < _updateInterval)
        return;
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

// ============= PROCESSORS ====================================================
void ServoMotor::calibrate(int minPulse, int maxPulse, std::optional<int> centerPulse)
{
    if (maxPulse <= minPulse)
        return;
    _minPulse = minPulse;
    _maxPulse = maxPulse;
    if (!centerPulse.has_value() && *centerPulse > minPulse && *centerPulse < maxPulse)
        _centerPulse = *centerPulse;
    else
        _centerPulse = (minPulse + maxPulse) / 2;
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
    _servo.writeMicroseconds(_currentPulse);
}

void ServoMotor::write(int angle)
{
    angle = constrain(angle, 0, 180);
    _currentAngle = angle;
    _currentPulse = angleToPulse(angle);
    _moving = false;
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

int ServoMotor::angleToPulse(
    int angle) const
{
    angle = constrain(angle, 0, 180);
    if (angle <= 90)
        return map(
            angle,
            0,
            90,
            _minPulse,
            _centerPulse);
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
        return map(
            pulse,
            _minPulse,
            _centerPulse,
            0,
            90);
    return map(
        pulse,
        _centerPulse,
        _maxPulse,
        90,
        180);
}

// ============= GETTERS | SETTERS ====================================================
void ServoMotor::setUpdateInterval(uint32_t intervalMs)
{
    _updateInterval = intervalMs;
}
void ServoMotor::center()
{
    writeMicroseconds(_centerPulse);
}

int ServoMotor::readAngle() const
{
    return _currentAngle;
}

int ServoMotor::readPulse() const
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