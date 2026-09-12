#include "ultrasonic.h"

namespace
{
constexpr uint8_t TRIG_PIN = 5;
constexpr uint8_t ECHO_PIN = 18;

constexpr uint8_t FILTER_SAMPLES = 5;
constexpr float MIN_DISTANCE_CM = 2.0f;
constexpr float MAX_DISTANCE_CM = 400.0f;
}

namespace Ultrasonic
{
void begin()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(TRIG_PIN, LOW);
}

float readRawDistanceCm()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    const unsigned long duration = pulseIn(ECHO_PIN, HIGH);

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
