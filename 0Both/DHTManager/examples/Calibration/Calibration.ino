#include <DHTManager.h>

DHTManager dht;
DHTSensor room(4, DHT_TYPE_22);

void setup()
{
    Serial.begin(115200);

    room.setTempOffset(-0.5f);
    room.setHumidityOffset(2.0f);

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
    }
}