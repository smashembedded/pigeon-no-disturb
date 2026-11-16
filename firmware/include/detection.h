#pragma once

#include <Arduino.h>

namespace Detection
{
enum class Event
{
    NONE,
    PRESENCE_DETECTED,
    DEPARTED
};

constexpr float DISTANCE_THRESHOLD_CM = 30.0f;
constexpr uint8_t CONFIRMATION_SAMPLES = 3;

void begin();
Event update(float distanceCm);
}
