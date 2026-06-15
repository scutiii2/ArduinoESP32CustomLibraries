#include <DHTManager.h>

DHTManager dht;
DHTSensor room(4, DHT_TYPE_22);

void tempHigh(DHTSensor& sensor)
{
    Serial.print("Temperature High: ");
    Serial.println(sensor.temperature());
}

void humidityLow(DHTSensor& sensor)
{
    Serial.print("Humidity Low: ");
    Serial.println(sensor.humidity());
}

void setup()
{
    Serial.begin(115200);

    room.onTemperatureAbove(35.0f, tempHigh);
    room.onHumidityBelow(30.0f, humidityLow);

    dht.add(room);
}

void loop()
{
    dht.update();
}