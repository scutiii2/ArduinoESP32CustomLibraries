#include "IRRemote.h"

void IRRemote::begin(uint8_t recvPin) {
    pin = recvPin;
    lastCode = 0;
    available = false;
    IrReceiver.begin(pin, ENABLE_LED_FEEDBACK);
}

void IRRemote::update() {
    if (IrReceiver.decode()) {
        lastCode = IrReceiver.decodedIRData.decodedRawData;
        available = true;
        IrReceiver.resume();
    }
}

bool IRRemote::read(uint32_t &code) {
    if (!available) return false;
    code = lastCode;
    available = false;
    return true;
}