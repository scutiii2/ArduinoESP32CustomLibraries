#include <RTOSLite.h>

RTOSLite rtos;

void blink() {
  digitalWrite(2, !digitalRead(2));
}

void sensorTask() {
  Serial.println("Sensor tick");
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);

  rtos.begin();

  rtos.add(500, blink, true);
  rtos.add(2000, sensorTask, true);
}

void loop() {
  rtos.update();
}