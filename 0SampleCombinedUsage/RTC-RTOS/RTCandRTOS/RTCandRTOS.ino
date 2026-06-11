#include <RTOSLite.h>
#include <RTCLite.h>

RTOSLite oRTOS;
RTCLite oRTC;

void taskRTC() {
  Serial.println(oRTC.toStringDT());
  Serial.println(oRTC.toStringTemp());
}

void setup() {
  Serial.begin(9600);
  oRTOS.add(1000, taskRTC, true);
}

void loop() {
  oRTOS.update(); // Place first
  oRTC.asyncBegin();
}