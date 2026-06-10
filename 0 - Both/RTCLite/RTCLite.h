#pragma once
#include <Arduino.h>
#include <RTClib.h>

struct RTCLite {
  RTC_DS3231 rtc;

  bool enabled = false;

  byte isCelsius = 1;

  byte trials = 5;
  unsigned long trialsInterval = 500;

  unsigned long lastAttempt = 0;
  byte attemptCount = 0;

  byte dayOfWeek = 0;
  byte values[7];
  float temp = 0;

  const char daysOfWeek[7][12] = {
    "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"
  };

  bool begin();
  void asyncBegin();

  void autoSet();
  void set(byte* dt, byte dow);

  byte* readDT();
  float readTemp();

  float toFahrenheit(float c);

  String toStringDT(bool prefix = true);
  String toStringTemp(bool prefix = true);
};