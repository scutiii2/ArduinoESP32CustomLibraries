#include <DHTManager.h>

DHTManager dht;
DHTSensor room(4, DHT_TYPE_22);

void setup()
{
    Serial.begin(115200);

    dht.add(room);
}

void loop()
{
    dht.update();

    if (room.updated())
    {
        Serial.print("Temperature: ");
        Serial.println(room.temperature());

        Serial.print("Humidity: ");
        Serial.println(room.humidity());

        Serial.print("Heat Index: ");
        Serial.println(room.heatIndex());

        Serial.print("Dew Point: ");
        Serial.println(room.dewPoint());
    }
}