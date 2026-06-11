#ifndef RTOSLITE_H
#define RTOSLITE_H

#include <Arduino.h>

class RTOSLite {
public:
  typedef void (*Callback)();

  static const uint8_t MAX_TASKS = 10;

  struct Task {
    uint32_t interval;
    uint32_t nextRun;
    bool repeat;
    bool active;
    Callback callback;
  };

  RTOSLite();

  void begin();
  int add(uint32_t interval, Callback cb, bool repeat = true);
  void remove(int id);
  void update();

private:
  Task tasks[MAX_TASKS];
};

#endif