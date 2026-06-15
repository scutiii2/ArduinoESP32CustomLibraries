#include "ServoManager.h"
#include "ServoMotor.h"

ServoMotor *ServoManager::_servos[SERVO_MAX_COUNT] = {nullptr};
uint8_t ServoManager::_count = 0;

bool ServoManager::add(ServoMotor &servo)
{
    // slot check
    if (_count >= SERVO_MAX_COUNT)
        return false;
    for (uint8_t i = 0; i < _count; i++)
        if (_servos[i] == &servo)
            return false;

    _servos[_count++] = &servo;
    return true;
}

bool ServoManager::remove(ServoMotor &servo)
{
    for (uint8_t i = 0; i < _count; i++)
    {
        if (_servos[i] != &servo)
            continue;
        for (uint8_t j = i; j < (_count - 1); j++)
            _servos[j] = _servos[j + 1];

        _servos[_count - 1] = nullptr;
        _count--;
        return true;
    }
    return false;
}

void ServoManager::update()
{
    for (uint8_t i = 0; i < _count; i++)
        if (_servos[i])
            _servos[i]->update();
}