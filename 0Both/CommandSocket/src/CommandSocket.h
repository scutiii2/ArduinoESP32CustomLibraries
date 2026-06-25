#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "CommandResult.h"

class CommandSocket
{
public:
    typedef CommandResult (*CommandHandler)(const String &command);

    CommandSocket(uint16_t port = 81);

    void begin(CommandHandler handler);
    void update();

    void send(uint8_t clientNum, const String &message);
    void broadcast(const String &message);

private:
    static CommandSocket *_instance;

    WebSocketsServer _socket;
    CommandHandler _handler;

    static void webSocketEvent(
        uint8_t clientNum,
        WStype_t type,
        uint8_t *payload,
        size_t length);

    void handleEvent(
        uint8_t clientNum,
        WStype_t type,
        uint8_t *payload,
        size_t length);
};