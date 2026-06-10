#include "RTCLite.h"

bool RTCLite::begin() {
  if (rtc.begin()) {
    enabled = true;
    return true;
  }
  return false;
}

void RTCLite::asyncBegin() {
  if (enabled) return;
  if (attemptCount >= trials) return;
  if (millis() - lastAttempt < trialsInterval) return;

  lastAttempt = millis();
  attemptCount++;

  if (rtc.begin()) {
    enabled = true;
  }
}

void RTCLite::autoSet() {
  if (!enabled) return;
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void RTCLite::set(byte* dt, byte dow) {
  if (!enabled) return;
  dayOfWeek = dow;
  rtc.adjust(DateTime((dt[6] * 100) + dt[5], dt[4], dt[3], dt[2], dt[1], dt[0]));
}

byte* RTCLite::readDT() {
  if (!enabled) return values;

  DateTime now = rtc.now();

  values[0] = now.second();
  values[1] = now.minute();
  values[2] = now.hour();
  values[3] = now.day();
  values[4] = now.month();
  values[5] = now.year() % 100;
  values[6] = now.year() / 100;

  dayOfWeek = now.dayOfTheWeek();

  return values;
}

float RTCLite::readTemp() {
  if (!enabled) return 0;

  temp = rtc.getTemperature();
  return isCelsius ? temp : toFahrenheit(temp);
}

float RTCLite::toFahrenheit(float c) {
  return (c * (9.0 / 5.0)) + 32.0;
}

String RTCLite::toStringDT(bool prefix) {
  readDT();

  String r = prefix ? "RTC Date Time  : " : "";
  r += String((values[6] * 100) + values[5]) + "/";
  r += String(values[4]) + "/";
  r += String(values[3]) + " (";
  r += String(daysOfWeek[dayOfWeek]) + ") ";
  r += String(values[2]) + ":";
  r += String(values[1]) + ":";
  r += String(values[0]);

  return r;
}

String RTCLite::toStringTemp(bool prefix) {
  readTemp();

  String r = prefix ? "RTC Temperature: " : "";
  r += String(temp);
  r += isCelsius ? " °C" : " °F";

  return r;
}