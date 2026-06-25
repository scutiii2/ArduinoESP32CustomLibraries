#pragma once

#include <Arduino.h>
#include <WebServer.h>

#include <Command.h>

class CommandHTTP
{
public:
    using CommandHandler = CommandResult (*)(const String &command);

    CommandHTTP(
        uint16_t port = 8080,
        const String &endpoint = "/api/command");

    void begin(CommandHandler handler);
    void update();

private:
    static CommandHTTP *_instance;

    WebServer _server;
    String _endpoint;
    CommandHandler _handler = nullptr;

    static void handleCommandStatic();

    void handleCommand();
    void sendCorsHeaders();
};