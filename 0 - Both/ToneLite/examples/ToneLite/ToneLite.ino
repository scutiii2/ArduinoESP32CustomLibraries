#include <ToneLite.h>

ToneLite buzzer;

ToneNote melody[] = {
  { NOTE_C4, 250 },
  { NOTE_D4, 250 },
  { NOTE_E4, 250 },
  { NOTE_F4, 250 },
  { NOTE_G4, 500 },
  { 0, 250 },
  { NOTE_G4, 500 }
};

void finished() {
  Serial.println("Done");
}

void setup() {
  Serial.begin(115200);

  buzzer.init(25);

  buzzer.load(melody, sizeof(melody) / sizeof(ToneNote));

  buzzer.onFinished(finished);

  buzzer.setTempo(1.0);

  buzzer.play(-1);
}

void loop() {
  buzzer.update();
}