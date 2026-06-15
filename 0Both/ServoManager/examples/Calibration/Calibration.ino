#include <ServoMotor.h>

ServoMotor servo;

int pulseWidth = 1500;

void printHelp()
{
    Serial.println();
    Serial.println("=== SERVO CALIBRATION ===");
    Serial.println("+ : +10 us");
    Serial.println("- : -10 us");
    Serial.println("> : +50 us");
    Serial.println("< : -50 us");
    Serial.println("c : center");
    Serial.println("m : minimum");
    Serial.println("x : maximum");
    Serial.println("s : show current pulse");
    Serial.println();
}

void moveServo()
{
    servo.writeMicroseconds(pulseWidth);

    Serial.print("Pulse: ");
    Serial.print(pulseWidth);
    Serial.println(" us");
}

void setup()
{
    Serial.begin(115200);

    servo.attach(18);

    moveServo();

    printHelp();
}

void loop()
{
    servo.update();

    if (!Serial.available())
    {
        return;
    }

    char cmd = Serial.read();

    switch (cmd)
    {
        case '+':
            pulseWidth += 10;
            moveServo();
            break;

        case '-':
            pulseWidth -= 10;
            moveServo();
            break;

        case '>':
            pulseWidth += 50;
            moveServo();
            break;

        case '<':
            pulseWidth -= 50;
            moveServo();
            break;

        case 'c':
            pulseWidth = 1500;
            moveServo();
            break;

        case 'm':
            pulseWidth = 500;
            moveServo();
            break;

        case 'x':
            pulseWidth = 2500;
            moveServo();
            break;

        case 's':
            Serial.print("Current Pulse: ");
            Serial.println(pulseWidth);
            break;
    }
}