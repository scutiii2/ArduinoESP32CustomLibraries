#include <Arduino.h>
#include <CommandController.h>

CommandController controller;

class ToggleLEDCommand : public Command {
  uint8_t pin_;

public:
  ToggleLEDCommand(uint8_t pin)
    : pin_(pin) {
    pinMode(pin_, OUTPUT);
  }

  std::string name() const override {
    return "toggle";
  }

  std::string description() const override {
    return "Toggle LED on/off";
  }

  std::string usage() const override {
    return "toggle [times] ~verbose";
  }

  std::vector<std::string> aliases() const override {
    return { "t" };
  }

  std::unordered_map<std::string, std::string> flagDescriptions() const override {
    return {
      { "verbose", "Print every toggle" }
    };
  }

  CommandResult execute(const Args &args, const CommandContext &ctx) override {
    int times = 1;

    if (!args.empty())
      times = atoi(args[0].c_str());

    for (int i = 0; i < times; i++) {
      digitalWrite(pin_, HIGH);
      delay(200);

      digitalWrite(pin_, LOW);
      delay(200);

      if (ctx.hasFlag("verbose")) {
        Serial.print("Toggle ");
        Serial.print(i + 1);
        Serial.print("/");
        Serial.println(times);
      }
    }

    CommandResult result;
    result.success = true;
    result.message =
      "Toggle completed. times=" + std::to_string(times);

    return result;
  }
};

void setup() {
  Serial.begin(115200);

  controller.registerCommand(
    new ToggleLEDCommand(13));

  auto result1 =
    controller.executeCommand(
      "toggle 3 ~verbose");

  Serial.println(result1.message.c_str());

  auto result2 =
    controller.executeCommand(
      "t 2");

  Serial.println(result2.message.c_str());

  auto result3 =
    controller.executeCommand(
      "help");

  Serial.println(result3.message.c_str());
}

void loop() {
}