#include <ServoManager.h>
#include <ServoMotor.h>

ServoMotor servo1;
ServoMotor servo2;
ServoMotor servo3;

void setup()
{
    servo1.write(0);
    servo2.write(90);
    servo3.write(180);
}

void loop()
{
    ServoManager::update();

    static uint32_t lastMove = 0;
    static bool state = false;

    if (millis() - lastMove >= 3000)
    {
        lastMove = millis();

        state = !state;

        if (state)
        {
            servo1.moveTo(180, 1000);
            servo2.moveTo(0, 1000);
            servo3.moveTo(90, 1000);
        }
        else
        {
            servo1.moveTo(0, 1000);
            servo2.moveTo(180, 1000);
            servo3.moveTo(90, 1000);
        }
    }
}