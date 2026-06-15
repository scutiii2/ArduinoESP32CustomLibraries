#include <DHTManager.h>

DHTManager dht;
DHTSensor room(4, DHT_TYPE_22);

void setup()
{
    Serial.begin(115200);

    room.setMedianSamples(3);
    room.setAverageSamples(5);

    dht.add(room);
}

void loop()
{
    dht.update();

    if (room.updated())
    {
        Serial.print("Filtered Temp: ");
        Serial.println(room.temperature());

        Serial.print("Filtered Humidity: ");
        Serial.println(room.humidity());
    }
}