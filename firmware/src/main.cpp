#include <Arduino.h>

namespace Pins
{
constexpr uint8_t TRIG = 5;
constexpr uint8_t ECHO = 18;
}

namespace Ultrasonic
{
constexpr uint8_t FILTER_SAMPLES = 5;

void begin()
{
    pinMode(Pins::TRIG, OUTPUT);
    pinMode(Pins::ECHO, INPUT);

    digitalWrite(Pins::TRIG, LOW);
}

float readRawDistanceCm()
{
    digitalWrite(Pins::TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(Pins::TRIG, HIGH);
    delayMicroseconds(10);

    digitalWrite(Pins::TRIG, LOW);

    const unsigned long duration = pulseIn(Pins::ECHO, HIGH);

    return duration / 58.0f;
}

float readDistanceCm()
{
    float sum = 0.0f;

    for (uint8_t i = 0; i < FILTER_SAMPLES; ++i)
    {
        sum += readRawDistanceCm();
        delay(10);
    }

    return sum / FILTER_SAMPLES;
}
}

void setup()
{
    Serial.begin(115200);

    Ultrasonic::begin();

    Serial.println();
    Serial.println("Pigeon No Disturb");
    Serial.println("Ultrasonic sensor ready");
}

void loop()
{
    const float distanceCm = Ultrasonic::readDistanceCm();

    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.println(" cm");

    delay(400);
}