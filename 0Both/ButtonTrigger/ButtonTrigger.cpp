#include "ButtonTrigger.h"

ButtonTrigger::ButtonTrigger(uint8_t pin, bool activeLow)
    : _pin(pin), _activeLow(activeLow) {}

void ButtonTrigger::begin() {
    pinMode(_pin, _activeLow ? INPUT_PULLUP : INPUT_PULLDOWN);
    _state = readButton();
    _lastState = _state;
}

bool ButtonTrigger::readButton() {
    bool value = digitalRead(_pin);

    if (_activeLow) {
        return !value;
    }

    return value;
}

void ButtonTrigger::onPress(Callback cb, uint32_t delayMs) {
    _onPress = cb;
    _pressDelay = delayMs;
}

void ButtonTrigger::onRelease(Callback cb, uint32_t delayMs) {
    _onRelease = cb;
    _releaseDelay = delayMs;
}

void ButtonTrigger::onHold(Callback cb, uint32_t holdMs, bool waitForRelease) {
    _onHold = cb;
    _holdDelay = holdMs;
    _waitForRelease = waitForRelease;
}

void ButtonTrigger::whileDown(Callback cb, uint32_t intervalMs) {
    _whileDown = cb;
    _whileInterval = intervalMs;
}

void ButtonTrigger::update() {
    uint32_t now = millis();

    _state = readButton();

    if (_state != _lastState) {
        if (_state) {
            _pressTime = now;

            _pressTriggered = false;
            _holdTriggered = false;
            _holdQualified = false;

            _lastWhileTime = now;
        } else {
            _releaseTime = now;
            _releaseTriggered = false;

            if (_holdQualified && _waitForRelease && !_holdTriggered) {
                _holdTriggered = true;

                if (_onHold) {
                    _onHold();
                }
            }
        }

        _lastState = _state;
    }

    if (_state) {
        if (!_pressTriggered && now - _pressTime >= _pressDelay) {
            _pressTriggered = true;

            if (_onPress) {
                _onPress();
            }
        }

        if (!_holdQualified && now - _pressTime >= _holdDelay) {
            _holdQualified = true;

            if (!_waitForRelease && !_holdTriggered) {
                _holdTriggered = true;

                if (_onHold) {
                    _onHold();
                }
            }
        }

        if (_whileDown && now - _lastWhileTime >= _whileInterval) {
            _lastWhileTime = now;
            _whileDown();
        }
    } else {
        if (!_releaseTriggered && now - _releaseTime >= _releaseDelay) {
            _releaseTriggered = true;

            if (_onRelease) {
                _onRelease();
            }
        }
    }
}