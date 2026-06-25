#include "CommandHTTP.h"

CommandHTTP *CommandHTTP::_instance = nullptr;

CommandHTTP::CommandHTTP(uint16_t port)
    : _server(port)
{
}

void CommandHTTP::begin(CommandHandler handler)
{
    _handler = handler;
    _instance = this;

    // Handle POST requests
    _server.on(
        "/command",
        HTTP_POST,
        handleCommandStatic);

    // Handle OPTIONS preflight requests for CORS
    _server.on(
        "/command",
        HTTP_OPTIONS,
        []()
        {
            _instance->_server.sendHeader("Access-Control-Allow-Origin", "*");
            _instance->_server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
            _instance->_server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
            _instance->_server.send(204); // No Content
        });

    _server.on(
        "/",
        HTTP_GET,
        []()
        {
            _instance->_server.sendHeader("Access-Control-Allow-Origin", "*");
            _instance->_server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            _instance->_server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
            _instance->_server.send(200, "text/plain", "OK");
        });

    _server.begin();
}

void CommandHTTP::update()
{
    _server.handleClient();
}

void CommandHTTP::handleCommandStatic()
{
    if (_instance)
    {
        _instance->handleCommand();
    }
}

void CommandHTTP::handleCommand()
{
    if (!_handler)
    {
        _server.sendHeader("Access-Control-Allow-Origin", "*");
        _server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

        _server.send(
            500,
            "text/plain",
            "ERROR|No handler");
        return;
    }

    String command = _server.arg("plain");
    command.trim();

    if (command.isEmpty())
    {
        _server.sendHeader("Access-Control-Allow-Origin", "*");
        _server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

        _server.send(
            400,
            "text/plain",
            "ERROR|Empty command");
        return;
    }

    CommandResult result = _handler(command);

    String response;
    response += result.success ? "OK|" : "ERROR|";
    response += result.message;

    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    _server.send(
        result.success ? 200 : 400,
        "text/plain",
        response);
}
