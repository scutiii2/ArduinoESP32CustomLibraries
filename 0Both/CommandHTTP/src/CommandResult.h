#pragma once

#include <Arduino.h>

struct CommandResult
{
    bool success = true;
    String message = "OK";
};