#pragma once

#include <Arduino.h>

class ButtonTrigger {
public:
    typedef void (*Callback)();

    ButtonTrigger(uint8_t pin, bool activeLow = true);

    void begin();
    void update();

    void onPress(Callback cb, uint32_t delayMs = 0);
    void onRelease(Callback cb, uint32_t delayMs = 0);
    void onHold(Callback cb, uint32_t holdMs = 1000, bool waitForRelease = true);
    void whileDown(Callback cb, uint32_t intervalMs = 100);

private:
    uint8_t _pin;
    bool _activeLow;

    bool _state = false;
    bool _lastState = false;

    uint32_t _pressTime = 0;
    uint32_t _releaseTime = 0;

    Callback _onPress = nullptr;
    Callback _onRelease = nullptr;
    Callback _onHold = nullptr;
    Callback _whileDown = nullptr;

    uint32_t _pressDelay = 0;
    uint32_t _releaseDelay = 0;
    uint32_t _holdDelay = 1000;
    uint32_t _whileInterval = 100;

    bool _pressTriggered = false;
    bool _releaseTriggered = false;
    bool _holdTriggered = false;
    bool _holdQualified = false;

    bool _waitForRelease = true;

    uint32_t _lastWhileTime = 0;

    bool readButton();
};