#include <DHTManager.h>

DHTManager dht;

DHTSensor sensor1(4, DHT_TYPE_22);
DHTSensor sensor2(5, DHT_TYPE_22);
DHTSensor sensor3(18, DHT_TYPE_22);

void setup()
{
    Serial.begin(115200);

    dht.add(sensor1);
    dht.add(sensor2);
    dht.add(sensor3);

    dht.setReadMode(DHT_SCHEDULED);
}

void loop()
{
    dht.update();

    if (sensor1.updated())
        Serial.println(sensor1.temperature());

    if (sensor2.updated())
        Serial.println(sensor2.temperature());

    if (sensor3.updated())
        Serial.println(sensor3.temperature());
}