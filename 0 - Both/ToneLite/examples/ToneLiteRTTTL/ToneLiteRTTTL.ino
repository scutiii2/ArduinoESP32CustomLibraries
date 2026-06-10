#include <ToneLite.h>
#include <ToneLiteRTTTL.h>

ToneLite buzzer;

ToneNote song[256];
size_t notes;

const char ringtone[] =
"Test:d=4,o=5,b=120:c,e,g,c6";

void setup() {
  buzzer.init(25);

  if (ToneLiteRTTTL::load(
        ringtone,
        song,
        256,
        notes
      )) {

    buzzer.load(song, notes);
    buzzer.play();
  }
}

void loop() {
  buzzer.update();
}