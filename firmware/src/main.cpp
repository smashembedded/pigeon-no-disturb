#include <Arduino.h>

#include "ultrasonic.h"
#include "detection.h"
#include "buzzer.h"


void setup()
{
    Serial.begin(115200);

    Ultrasonic::begin();
    Detection::begin();
    Buzzer::begin();


    Serial.println();
    Serial.println("Pigeon No Disturb");
    Serial.println("Ultrasonic sensor ready");

    Serial.print("Detection threshold: ");
    Serial.print(Detection::DISTANCE_THRESHOLD_CM);
    Serial.println(" cm");

}

void loop()
{
    const float distanceCm = Ultrasonic::readDistanceCm();

    if (distanceCm >= 0.0f)
    {
        Serial.print("Distance: ");
        Serial.print(distanceCm);
        Serial.println(" cm");

        Detection::update(distanceCm);

        Buzzer::on();
        delay(2000);
        Buzzer::off();
    }

    delay(400);
}