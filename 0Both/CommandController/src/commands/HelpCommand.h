#pragma once
#include "../Command.h"

class CommandController;

class HelpCommand : public Command
{
    CommandController *controller;

public:
    HelpCommand(CommandController *ctrl);
    std::string name() const override;
    std::string description() const override;
    std::vector<std::string> aliases() const override;
    CommandResult execute(const Args &args, const CommandContext &ctx) override;
};
