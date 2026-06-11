#include "ToneLite.h"

void ToneLite::init(uint8_t tonePin
#ifdef ESP32
                    , uint8_t toneChannel
#endif
) {
  pin = tonePin;

#ifdef ESP32
  channel = toneChannel;
  ledcAttach(pin, 5000, 8);
#endif
}

void ToneLite::load(const ToneNote* melody, size_t length) {
  notes = melody;
  count = length;
}

void ToneLite::play(int repeat) {
  if (!notes || !count) return;

  repeats = repeat;
  played = 0;
  current = 0;

  playing = true;
  paused = false;

  startNote();
}

void ToneLite::pause() {
  paused = true;

#ifdef ESP32
  ledcWriteTone(channel, 0);
#else
  noTone(pin);
#endif
}

void ToneLite::resume() {
  if (!paused) return;

  paused = false;
  startNote();
}

void ToneLite::stop() {
  playing = false;
  paused = false;

#ifdef ESP32
  ledcWriteTone(channel, 0);
#else
  noTone(pin);
#endif
}

void ToneLite::seek(size_t index) {
  if (index >= count) return;

  current = index;
  startNote();
}

void ToneLite::update() {
  if (!playing || paused) return;

  if (millis() - lastTime < notes[current].duration / tempo) return;

  current++;

  if (current >= count) {
    played++;

    if (repeats == -1 || played < repeats) {
      current = 0;
    } else {
      stop();

      if (finishedCallback) {
        finishedCallback();
      }

      return;
    }
  }

  startNote();
}

bool ToneLite::isPlaying() {
  return playing;
}

bool ToneLite::isPaused() {
  return paused;
}

size_t ToneLite::position() {
  return current;
}

void ToneLite::setTempo(float multiplier) {
  tempo = multiplier;
}

#ifdef ESP32
void ToneLite::setVolume(uint8_t percent) {
  volume = constrain(percent, 0, 100);
}
#endif

void ToneLite::onFinished(void (*callback)()) {
  finishedCallback = callback;
}

void ToneLite::startNote() {
  lastTime = millis();

  uint16_t freq = notes[current].frequency;

#ifdef ESP32
  ledcWrite(channel, map(volume, 0, 100, 0, 255));
  ledcWriteTone(channel, freq);
#else
  if (freq == 0) {
    noTone(pin);
  } else {
    tone(pin, freq);
  }
#endif
}