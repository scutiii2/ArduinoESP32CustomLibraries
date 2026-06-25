#include <Arduino.h>
#include <CommandController.h>
#include <Command.h>
#include "commands/BlinkCommand.h"

CommandController controller;

void setup() {
  Serial.begin(9600);
  controller.registerCommand(new BlinkCommand(13));
}

void loop() {
  CommandResult result = controller.executeCommand(
    "blink 5 250 --fast ~verbose");
  Serial.println(result.message.c_str());

  //   controller.executeCommand("flash 2 300 --slow");
  //   controller.executeCommand("help");
  delay(5000);
}
