#include <ServoMotor.h>

ServoMotor servo;

void setup()
{
    servo.attach(18);
    servo.write(0);
}

void loop()
{
    servo.update();

    static uint32_t lastMove = 0;
    static bool state = false;

    if (millis() - lastMove >= 2000)
    {
        lastMove = millis();

        state = !state;

        servo.write(state ? 180 : 0);
    }
}