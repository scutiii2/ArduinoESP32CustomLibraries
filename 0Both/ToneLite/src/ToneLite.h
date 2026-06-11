#pragma once

#include <Arduino.h>
#include "ToneLiteNotes.h"

struct ToneNote {
  uint16_t frequency;
  uint16_t duration;
};

struct ToneLite {
  uint8_t pin;

#ifdef ESP32
  uint8_t channel;
  uint8_t volume = 100;
#endif

  const ToneNote* notes = nullptr;

  size_t count = 0;
  size_t current = 0;

  bool playing = false;
  bool paused = false;

  int repeats = 1;
  int played = 0;

  float tempo = 1.0f;

  unsigned long lastTime = 0;

  void (*finishedCallback)() = nullptr;

  void init(uint8_t tonePin
#ifdef ESP32
            , uint8_t toneChannel = 0
#endif
  );

  void load(const ToneNote* melody, size_t length);

  void play(int repeat = 1);

  void pause();

  void resume();

  void stop();

  void seek(size_t index);

  void update();

  bool isPlaying();

  bool isPaused();

  size_t position();

  void setTempo(float multiplier);

#ifdef ESP32
  void setVolume(uint8_t percent);
#endif

  void onFinished(void (*callback)());

private:
  void startNote();
};