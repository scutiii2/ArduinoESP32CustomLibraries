#include <DHTManager.h>

DHTManager dht;

DHTSensor room(4, DHT_TYPE_22);
DHTSensor outside(5, DHT_TYPE_22);
DHTSensor greenhouse(18, DHT_TYPE_11);

void setup()
{
    Serial.begin(115200);

    dht.add(room);
    dht.add(outside);
    dht.add(greenhouse);
}

void loop()
{
    dht.update();

    if (room.updated())
    {
        Serial.print("Room: ");
        Serial.print(room.temperature());
        Serial.print("C ");
        Serial.print(room.humidity());
        Serial.println("%");
    }

    if (outside.updated())
    {
        Serial.print("Outside: ");
        Serial.print(outside.temperature());
        Serial.print("C ");
        Serial.print(outside.humidity());
        Serial.println("%");
    }

    if (greenhouse.updated())
    {
        Serial.print("Greenhouse: ");
        Serial.print(greenhouse.temperature());
        Serial.print("C ");
        Serial.print(greenhouse.humidity());
        Serial.println("%");
    }
}