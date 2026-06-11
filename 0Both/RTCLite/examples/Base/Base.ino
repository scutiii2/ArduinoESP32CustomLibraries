#include <RTCLite.h>

RTCLite oRTC;

void setup() {
  Serial.begin(9600);
}

void loop() {
  oRTC.asyncBegin();

  Serial.println(oRTC.toStringDT());
  Serial.println(oRTC.toStringTemp());
}