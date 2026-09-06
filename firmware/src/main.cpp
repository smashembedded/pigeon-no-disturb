#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("Pigeon No Disturb");
    Serial.println("System starting...");
}

void loop()
{
    delay(1000);
}