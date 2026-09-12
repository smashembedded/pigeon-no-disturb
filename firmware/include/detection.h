#pragma once

#include <Arduino.h>

namespace Detection
{
constexpr float DISTANCE_THRESHOLD_CM = 30.0f;
constexpr uint8_t CONFIRMATION_SAMPLES = 3;

void begin();
void update(float distanceCm);
}
