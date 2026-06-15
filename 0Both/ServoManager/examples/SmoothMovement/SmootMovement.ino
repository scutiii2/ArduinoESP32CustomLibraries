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

    if (millis() - lastMove >= 3000)
    {
        lastMove = millis();

        state = !state;

        servo.moveTo(
            state ? 180 : 0,
            1000
        );
    }
}