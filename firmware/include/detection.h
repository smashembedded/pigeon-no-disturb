#pragma once

namespace Detection
{
constexpr float DISTANCE_THRESHOLD_CM = 50.0f;

void begin();
void update(float distanceCm);
}
