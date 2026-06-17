#pragma once

#include <Arduino.h>
#include "ServoMotor.h"

#define SERVO_MAX_COUNT 32

class ServoManager
{
public:
    static bool add(ServoMotor& servo);
    static bool remove(ServoMotor& servo);
    static void update();
    
    static uint8_t count();
    static ServoMotor* get(uint8_t index);

private:
    static ServoMotor* _servos[SERVO_MAX_COUNT];
    static uint8_t _count;
};