#pragma once
#include "../Command.h"
#include <Arduino.h>

class BlinkCommand : public Command
{
    uint8_t pin_;

public:
    BlinkCommand(uint8_t pin) : pin_(pin)
    {
        pinMode(pin_, OUTPUT);
    }

    std::string name() const override
    {
        return "blink";
    }

    std::string description() const override
    {
        return "Blink LED";
    }

    std::string usage() const override
    {
        return "blink [times] [delay_ms] --fast --slow ~verbose";
    }

    std::vector<std::string> aliases() const override
    {
        return {"flash"};
    }

    std::unordered_map<std::string, std::string> subcommandDescriptions() const override
    {
        return {
            {"fast", "Use 50ms delay"},
            {"slow", "Use 1000ms delay"}};
    }

    std::unordered_map<std::string, std::string> flagDescriptions() const override
    {
        return {
            {"verbose", "Print each blink"}};
    }

    CommandResult execute(const Args &args, const CommandContext &ctx) override
    {
        int times = 3;
        int delayMs = 200;

        if (args.size() > 0)
            times = atoi(args[0].c_str());

        if (args.size() > 1)
            delayMs = atoi(args[1].c_str());

        if (ctx.hasSubcommand("fast"))
            delayMs = 50;

        if (ctx.hasSubcommand("slow"))
            delayMs = 1000;

        for (int i = 0; i < times; i++)
        {
            if (ctx.hasFlag("verbose"))
            {
                Serial.print("Blink ");
                Serial.print(i + 1);
                Serial.print("/");
                Serial.println(times);
            }

            digitalWrite(pin_, HIGH);
            delay(delayMs);

            digitalWrite(pin_, LOW);
            delay(delayMs);
        }

        CommandResult result;
        result.success = true;

        result.message =
            "Blink completed. "
            "times=" +
            std::to_string(times) +
            ", delayMs=" + std::to_string(delayMs);

        return result;
    }
};