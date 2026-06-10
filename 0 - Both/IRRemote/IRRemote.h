#pragma once
#include <Arduino.h>
#include <IRremote.hpp>

struct IRRemote {
    uint8_t pin;
    uint32_t lastCode;
    bool available;

    void begin(uint8_t recvPin);
    void update();
    bool read(uint32_t &code);
};