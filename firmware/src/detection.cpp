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

bool isDetected(float distanceCm)
{
    return distanceCm > 0.0f &&
           distanceCm <= Detection::DISTANCE_THRESHOLD_CM;
}
}

namespace Detection
{
void begin()
{
    state = State::CLEAR;
    confirmationSamples = 0;
}

void update(float distanceCm)
{
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

            if (confirmationSamples >= CONFIRMATION_SAMPLES)
            {
                state = State::PRESENT;
                confirmationSamples = 0;

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
            state = State::CLEAR;

            Serial.println("Event: DEPARTED");
        }
        break;
    }
}
}