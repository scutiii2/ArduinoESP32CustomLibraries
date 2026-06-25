#pragma once
#include "Command.h"
#include <vector>
#include <string>

class CommandController
{
    struct CmdEntry
    {
        Command *cmd;
        std::string nameLower;
        std::vector<std::string> aliasesLower;
    };
    std::vector<CmdEntry> commands;

    static std::string toLower(const std::string &s);

public:
    CommandController();
    ~CommandController();

    size_t commandCount() const;
    void registerCommand(Command *cmd);
    void unregisterCommand(const std::string &name);
    CommandResult executeCommand(const std::string &input);
    void printHelp() const;
};
