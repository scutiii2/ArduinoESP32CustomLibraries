#pragma once

#include <Arduino.h>
#include "ToneLite.h"

struct ToneLiteRTTTL {
  static bool load(
    const char* rtttl,
    ToneNote* buffer,
    size_t maxNotes,
    size_t& noteCount
  );

private:
  static uint16_t getFrequency(char note, int octave, bool sharp);
};