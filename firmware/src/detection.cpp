#include "detection.h"

#include <Arduino.h>

namespace
{
enum class State
{
    CLEAR,
    PRESENT
};

State state = State::CLEAR;

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
}

void update(float distanceCm)
{
    const bool detected = isDetected(distanceCm);

    switch (state)
    {
    case State::CLEAR:
        if (detected)
        {
            state = State::PRESENT;
            Serial.println("Event: PRESENCE_DETECTED");
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
