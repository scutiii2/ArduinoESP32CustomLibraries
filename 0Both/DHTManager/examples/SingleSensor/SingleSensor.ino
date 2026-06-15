#include <DHTManager.h>

DHTManager dht;
DHTSensor room(4, DHT_TYPE_22);

void setup()
{
    Serial.begin(115200);

    dht.add(room);

    room.read();
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
    }
}