#include "buzzer.h"

#include <Arduino.h>

namespace
{
constexpr uint8_t BUZZER_PIN = 23;
constexpr uint8_t BUZZER_CHANNEL = 0;
constexpr uint32_t BUZZER_FREQUENCY = 2000;
constexpr uint8_t BUZZER_RESOLUTION = 8;
}

namespace Buzzer
{
void begin()
{
    ledcSetup(
        BUZZER_CHANNEL,
        BUZZER_FREQUENCY,
        BUZZER_RESOLUTION);

    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

    off();
}

void on()
{
    ledcWriteTone(BUZZER_CHANNEL, BUZZER_FREQUENCY);
}

void off()
{
    ledcWriteTone(BUZZER_CHANNEL, 0);
}
}