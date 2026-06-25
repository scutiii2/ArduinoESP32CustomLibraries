#pragma once

#include <Arduino.h>
#include <WebServer.h>

#include "CommandResult.h"

class CommandHTTP
{
public:
    typedef CommandResult (*CommandHandler)(const String &command);

    CommandHTTP(uint16_t port = 80);

    void begin(CommandHandler handler);
    void update();

private:
    static CommandHTTP *_instance;

    WebServer _server;
    CommandHandler _handler;

    static void handleCommandStatic();
    void handleCommand();
};