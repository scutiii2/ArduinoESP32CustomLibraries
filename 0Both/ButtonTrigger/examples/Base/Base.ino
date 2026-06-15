#include "ButtonTrigger.h"

ButtonTrigger button(2);

void pressed() {
    Serial.println("Pressed");
}

void released() {
    Serial.println("Released");
}

void held() {
    Serial.println("Held");
}

void repeat() {
    Serial.println("While Down");
}

void setup() {
    Serial.begin(115200);

    button.begin();

    button.onPress(pressed, 50);
    button.onRelease(released, 50);
    button.onHold(held, 1000, true);
    button.whileDown(repeat, 200);
}

void loop() {
    button.update();
}