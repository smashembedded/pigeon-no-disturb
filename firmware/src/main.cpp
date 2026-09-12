#include <Arduino.h>

namespace Pins
{
constexpr uint8_t TRIG = 5;
constexpr uint8_t ECHO = 18;
}

namespace Detection
{
constexpr float DISTANCE_THRESHOLD_CM = 50.0f;

bool isDetected(float distanceCm)
{
    return distanceCm > 0.0f &&
           distanceCm <= DISTANCE_THRESHOLD_CM;
}
}

namespace Ultrasonic
{
constexpr uint8_t FILTER_SAMPLES = 5;
constexpr float MIN_DISTANCE_CM = 2.0f;
constexpr float MAX_DISTANCE_CM = 400.0f;

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

    if (duration == 0)
    {
        return -1.0f;
    }

    const float distanceCm = duration / 58.0f;

    if (distanceCm < MIN_DISTANCE_CM ||
        distanceCm > MAX_DISTANCE_CM)
    {
        return -1.0f;
    }

    return distanceCm;
}

float readDistanceCm()
{
    float sum = 0.0f;
    uint8_t validSamples = 0;

    for (uint8_t i = 0; i < FILTER_SAMPLES; ++i)
    {
        const float distanceCm = readRawDistanceCm();

        if (distanceCm >= 0.0f)
        {
            sum += distanceCm;
            ++validSamples;
        }

        delay(10);
    }

    if (validSamples == 0)
    {
        return -1.0f;
    }

    return sum / validSamples;
}
}

void setup()
{
    Serial.begin(115200);

    Ultrasonic::begin();

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
        const bool detected = Detection::isDetected(distanceCm);

        Serial.print("Distance: ");
        Serial.print(distanceCm);
        Serial.print(" cm | Status: ");

        if (detected)
        {
            Serial.println("DETECTED");
        }
        else
        {
            Serial.println("CLEAR");
        }
    }

    delay(400);
}