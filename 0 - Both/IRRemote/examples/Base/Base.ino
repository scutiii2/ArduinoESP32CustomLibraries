#include "IRRemote.h"

IRRemote oRemote;
uint32_t code;

void setup() {
    Serial.begin(115200);
    oRemote.begin(15);
}

void loop() {
    oRemote.update();

    if (oRemote.read(code)) {
        Serial.println(code, HEX);
    }
}