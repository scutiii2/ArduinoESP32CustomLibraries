#include <WiFi.h>

#include <CommandHTTP.h>
#include <CommandController.h>

const char *ssid = "YOUR_WIFI";
const char *password = "YOUR_PASSWORD";

CommandController controller;

CommandHTTP http;

CommandResult executeCommand(
  const String &command) {
  return controller.executeCommand(
    std::string(command.c_str()));
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(
    ssid,
    password);

  while (WiFi.status() != WL_CONNECTED) {
  }

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Serial.println(
    "Endpoint: http://" + WiFi.localIP().toString() + ":8080/api/command");

  http.begin(executeCommand);
}

void loop() {
  http.update();
}