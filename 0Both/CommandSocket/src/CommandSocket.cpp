#include "CommandSocket.h"

CommandSocket *CommandSocket::_instance = nullptr;

CommandSocket::CommandSocket(uint16_t port)
    : _socket(port)
{
}

void CommandSocket::begin(CommandHandler handler)
{
    _handler = handler;

    _instance = this;

    _socket.begin();
    _socket.onEvent(webSocketEvent);
}

void CommandSocket::update()
{
    _socket.loop();
}

void CommandSocket::send(
    uint8_t clientNum,
    const String &message)
{
    String payload = message;
    _socket.sendTXT(clientNum, payload);
}

void CommandSocket::broadcast(
    const String &message)
{
    String payload = message;
    _socket.broadcastTXT(payload);
}

void CommandSocket::webSocketEvent(
    uint8_t clientNum,
    WStype_t type,
    uint8_t *payload,
    size_t length)
{
    if (_instance)
    {
        _instance->handleEvent(
            clientNum,
            type,
            payload,
            length);
    }
}

void CommandSocket::handleEvent(
    uint8_t clientNum,
    WStype_t type,
    uint8_t *payload,
    size_t length)
{
    if (!_handler)
        return;

    if (type != WStype_TEXT)
        return;

    CommandResult result =
        _handler((char *)payload);

    String response;

    response += result.success ? "OK|" : "ERROR|";
    response += result.message.c_str();

    _socket.sendTXT(
        clientNum,
        response);
}