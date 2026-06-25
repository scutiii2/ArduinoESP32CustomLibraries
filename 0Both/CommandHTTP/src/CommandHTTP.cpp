#include "CommandHTTP.h"

CommandHTTP *CommandHTTP::_instance = nullptr;

CommandHTTP::CommandHTTP(
    uint16_t port,
    const String &endpoint)
    : _server(port),
      _endpoint(endpoint)
{
}

void CommandHTTP::begin(CommandHandler handler)
{
    _handler = handler;
    _instance = this;

    _server.on(
        _endpoint.c_str(),
        HTTP_POST,
        handleCommandStatic);

    _server.on(
        _endpoint.c_str(),
        HTTP_OPTIONS,
        []()
        {
            _instance->sendCorsHeaders();
            _instance->_server.send(204);
        });

    _server.on(
        "/",
        HTTP_GET,
        []()
        {
            _instance->sendCorsHeaders();

            _instance->_server.send(
                200,
                "text/plain",
                "OK");
        });

    _server.onNotFound(
        []()
        {
            if (_instance->_server.method() == HTTP_OPTIONS)
            {
                _instance->sendCorsHeaders();
                _instance->_server.send(204);
                return;
            }

            _instance->sendCorsHeaders();

            _instance->_server.send(
                404,
                "text/plain",
                "ERROR|Not Found");
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

void CommandHTTP::sendCorsHeaders()
{
    _server.sendHeader(
        "Access-Control-Allow-Origin",
        "*");

    _server.sendHeader(
        "Access-Control-Allow-Methods",
        "GET, POST, OPTIONS");

    _server.sendHeader(
        "Access-Control-Allow-Headers",
        "Content-Type");
}

void CommandHTTP::handleCommand()
{
    sendCorsHeaders();

    if (!_handler)
    {
        _server.send(
            500,
            "text/plain",
            "ERROR|No handler");

        return;
    }

    String command =
        _server.arg("plain");

    command.trim();

    if (command.isEmpty())
    {
        _server.send(
            400,
            "text/plain",
            "ERROR|Empty command");

        return;
    }

    CommandResult result =
        _handler(command);

    String response;

    response.reserve(
        result.message.length() + 8);

    response =
        result.success
            ? "OK|"
            : "ERROR|";

    response += result.message.c_str();

    _server.send(
        result.success
            ? 200
            : 400,
        "text/plain",
        response);
}