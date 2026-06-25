#include "HelpCommand.h"
#include "../CommandController.h"

HelpCommand::HelpCommand(CommandController *ctrl)
    : controller(ctrl)
{
}

std::string HelpCommand::name() const
{
    return "help";
}

std::string HelpCommand::description() const
{
    return "Displays all commands";
}

std::vector<std::string> HelpCommand::aliases() const
{
    return {"h", "?"};
}

CommandResult HelpCommand::execute(const Args &, const CommandContext &)
{
    controller->printHelp();

    CommandResult result;
    result.success = true;
    result.message = "Displayed help.";

    return result;
}