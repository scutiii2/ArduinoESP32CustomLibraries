#include "RTOSLite.h"

RTOSLite::RTOSLite() {}

void RTOSLite::begin() {
  for (uint8_t i = 0; i < MAX_TASKS; i++) {
    tasks[i].active = false;
  }
}

int RTOSLite::add(uint32_t interval, Callback cb, bool repeat) {
  for (uint8_t i = 0; i < MAX_TASKS; i++) {
    if (!tasks[i].active) {
      tasks[i].interval = interval;
      tasks[i].callback = cb;
      tasks[i].repeat = repeat;
      tasks[i].nextRun = millis() + interval;
      tasks[i].active = true;
      return i;
    }
  }
  return -1;
}

void RTOSLite::remove(int id) {
  if (id >= 0 && id < MAX_TASKS) {
    tasks[id].active = false;
  }
}

void RTOSLite::update() {
  uint32_t now = millis();

  for (uint8_t i = 0; i < MAX_TASKS; i++) {
    if (!tasks[i].active) continue;

    if ((int32_t)(now - tasks[i].nextRun) >= 0) {
      tasks[i].callback();

      if (tasks[i].repeat) {
        tasks[i].nextRun += tasks[i].interval;
      } else {
        tasks[i].active = false;
      }
    }
  }
}