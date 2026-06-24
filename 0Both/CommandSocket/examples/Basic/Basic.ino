#include <WiFi.h>

#include <CommandSocket.h>
#include <CommandController.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

CommandController controller;
CommandSocket socket;

CommandResult executeCommand(
    const String& command
)
{
    controller.executeCommand(
        std::string(command.c_str())
    );

    return {
        true,
        "Executed"
    };
}

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
    }

    Serial.println(WiFi.localIP());

    socket.begin(executeCommand);
}

void loop()
{
    socket.update();
}