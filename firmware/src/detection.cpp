#include "detection.h"

#include <Arduino.h>

namespace
{
enum class State
{
    CLEAR,
    CONFIRMING,
    PRESENT
};

State state = State::CLEAR;
uint8_t confirmationSamples = 0;
unsigned long presenceStartTime = 0;

bool isValid(float distanceCm)
{
    return distanceCm >= 0.0f;
}

bool isDetected(float distanceCm)
{
    return isValid(distanceCm) &&
           distanceCm <= Detection::DISTANCE_THRESHOLD_CM;
}
}

namespace Detection
{
void begin()
{
    state = State::CLEAR;
    confirmationSamples = 0;
    presenceStartTime = 0;
}

void update(float distanceCm)
{
    // Invalid measurements must not change the detection state.
    if (!isValid(distanceCm))
    {
        return;
    }

    const bool detected = isDetected(distanceCm);

    switch (state)
    {
    case State::CLEAR:
        if (detected)
        {
            confirmationSamples = 1;
            state = State::CONFIRMING;
        }
        break;

    case State::CONFIRMING:
        if (detected)
        {
            ++confirmationSamples;

            if (confirmationSamples >= Detection::CONFIRMATION_SAMPLES)
            {
                state = State::PRESENT;
                confirmationSamples = 0;
                presenceStartTime = millis();

                Serial.println("Event: PRESENCE_DETECTED");
            }
        }
        else
        {
            confirmationSamples = 0;
            state = State::CLEAR;
        }
        break;

    case State::PRESENT:
        if (!detected)
        {
            const unsigned long presenceDuration =
                millis() - presenceStartTime;

            state = State::CLEAR;

            Serial.print("Event: DEPARTED");
            Serial.print(" | Presence duration: ");
            Serial.print(presenceDuration);
            Serial.println(" ms");
        }
        break;
    }
}
}