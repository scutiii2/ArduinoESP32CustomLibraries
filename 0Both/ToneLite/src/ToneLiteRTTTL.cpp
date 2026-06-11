#include "ToneLiteRTTTL.h"

uint16_t ToneLiteRTTTL::getFrequency(char note, int octave, bool sharp) {
  static const uint16_t table[12] = {
    262,
    277,
    294,
    311,
    330,
    349,
    370,
    392,
    415,
    440,
    466,
    494
  };

  if (note == 'p') return 0;

  int index = 0;

  switch (note) {
    case 'c': index = 0; break;
    case 'd': index = 2; break;
    case 'e': index = 4; break;
    case 'f': index = 5; break;
    case 'g': index = 7; break;
    case 'a': index = 9; break;
    case 'b': index = 11; break;
  }

  if (sharp) index++;

  float freq = table[index];

  int shift = octave - 4;

  while (shift > 0) {
    freq *= 2.0;
    shift--;
  }

  while (shift < 0) {
    freq /= 2.0;
    shift++;
  }

  return (uint16_t)freq;
}

bool ToneLiteRTTTL::load(
  const char* rtttl,
  ToneNote* buffer,
  size_t maxNotes,
  size_t& noteCount
) {
  noteCount = 0;

  if (!rtttl) return false;

  while (*rtttl && *rtttl != ':') rtttl++;

  if (*rtttl == ':') rtttl++;

  int defaultDuration = 4;
  int defaultOctave = 6;
  int bpm = 63;

  while (*rtttl && *rtttl != ':') {
    if (*rtttl == 'd') {
      rtttl += 2;
      defaultDuration = atoi(rtttl);
    }

    if (*rtttl == 'o') {
      rtttl += 2;
      defaultOctave = atoi(rtttl);
    }

    if (*rtttl == 'b') {
      rtttl += 2;
      bpm = atoi(rtttl);
    }

    while (*rtttl && *rtttl != ',' && *rtttl != ':') rtttl++;

    if (*rtttl == ',') rtttl++;
  }

  if (*rtttl == ':') rtttl++;

  int wholeNote = (60000 * 4) / bpm;

  while (*rtttl && noteCount < maxNotes) {
    int duration = 0;

    while (isdigit(*rtttl)) {
      duration = duration * 10 + (*rtttl - '0');
      rtttl++;
    }

    if (duration == 0) {
      duration = defaultDuration;
    }

    char note = tolower(*rtttl++);

    bool sharp = false;

    if (*rtttl == '#') {
      sharp = true;
      rtttl++;
    }

    bool dotted = false;

    if (*rtttl == '.') {
      dotted = true;
      rtttl++;
    }

    int octave = defaultOctave;

    if (isdigit(*rtttl)) {
      octave = *rtttl - '0';
      rtttl++;
    }

    int ms = wholeNote / duration;

    if (dotted) {
      ms += ms / 2;
    }

    buffer[noteCount].frequency =
      getFrequency(note, octave, sharp);

    buffer[noteCount].duration = ms;

    noteCount++;

    while (*rtttl == ',') {
      rtttl++;
    }
  }

  return noteCount > 0;
}