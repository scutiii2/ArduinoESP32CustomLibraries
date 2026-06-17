#include <ServoManager.h>
#include <ServoMotor.h>

ServoMotor servo1(18);
ServoMotor servo2(19);
ServoMotor servo3(21);

ServoMotor* servos[3] = { &servo1, &servo2, &servo3 };

int pulseWidth[3] = {1500, 1500, 1500};  // one pulse width per servo
uint8_t currentServo = 0;                // index of selected servo

void printHelp() {
    Serial.println();
    Serial.println("=== SERVO CALIBRATION ===");
    Serial.println("Select servo: 1, 2, 3");
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

void moveServo(uint8_t i, int pulse) {
    servos[i]->writeMicroseconds(pulse);
    Serial.print("Servo ");
    Serial.print(i + 1);
    Serial.print(" Pulse: ");
    Serial.print(pulse);
    Serial.println(" us");
}

void setup() {
    Serial.begin(115200);
    moveServo(currentServo, pulseWidth[currentServo]);
    printHelp();
}

void loop() {
    if (!Serial.available()) return;
    char cmd = Serial.read();

    switch (cmd) {
    case '1': case '2': case '3':
        currentServo = cmd - '1';  // select servo index
        Serial.print("Selected Servo ");
        Serial.println(currentServo + 1);
        break;

    case '+':
        pulseWidth[currentServo] += 10;
        moveServo(currentServo, pulseWidth[currentServo]);
        break;

    case '-':
        pulseWidth[currentServo] -= 10;
        moveServo(currentServo, pulseWidth[currentServo]);
        break;

    case '>':
        pulseWidth[currentServo] += 50;
        moveServo(currentServo, pulseWidth[currentServo]);
        break;

    case '<':
        pulseWidth[currentServo] -= 50;
        moveServo(currentServo, pulseWidth[currentServo]);
        break;

    case 'c':
        pulseWidth[currentServo] = 1500;
        moveServo(currentServo, pulseWidth[currentServo]);
        break;

    case 'm':
        pulseWidth[currentServo] = 500;
        moveServo(currentServo, pulseWidth[currentServo]);
        break;

    case 'x':
        pulseWidth[currentServo] = 2500;
        moveServo(currentServo, pulseWidth[currentServo]);
        break;

    case 's':
        Serial.print("Servo ");
        Serial.print(currentServo + 1);
        Serial.print(" Current Pulse: ");
        Serial.println(pulseWidth[currentServo]);
        break;
    }
}
