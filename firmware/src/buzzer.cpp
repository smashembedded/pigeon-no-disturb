#include "buzzer.h"
#include <Arduino.h>

namespace
{
constexpr uint8_t BUZZER_PIN = 23;
constexpr uint8_t BUZZER_RESOLUTION = 10;
constexpr uint32_t BUZZER_DUTY = 0x1FF;

constexpr uint32_t START_FREQUENCY_HZ = 2700;
constexpr uint32_t PEAK_FREQUENCY_HZ = 3100;
constexpr uint32_t END_FREQUENCY_HZ = 2500;

constexpr uint16_t SWEEP_DURATION_MS = 750;
constexpr uint16_t STEP_DURATION_MS = 10;

constexpr uint8_t PATTERN_REPETITIONS = 3;
constexpr uint16_t REPETITION_PAUSE_MS = 40;

bool setFrequency(uint32_t frequency)
{
    const uint32_t actualFrequency =
        ledcChangeFrequency(
            BUZZER_PIN,
            frequency,
            BUZZER_RESOLUTION
        );

    if (actualFrequency == 0)
    {
        Serial.print("ERROR: Could not set frequency: ");
        Serial.println(frequency);

        ledcWrite(BUZZER_PIN, 0);
        return false;
    }

    return ledcWrite(BUZZER_PIN, BUZZER_DUTY);
}

bool playSweep(
    uint32_t startFrequency,
    uint32_t peakFrequency,
    uint32_t endFrequency)
{
    const uint16_t steps =
        SWEEP_DURATION_MS / STEP_DURATION_MS;

    const uint16_t halfSteps = steps / 2;

    // Ascending sweep
    for (uint16_t i = 0; i < halfSteps; ++i)
    {
        const float progress =
            static_cast<float>(i) / halfSteps;

        const int32_t delta =
            static_cast<int32_t>(peakFrequency) -
            static_cast<int32_t>(startFrequency);

        const uint32_t frequency =
            static_cast<uint32_t>(
                static_cast<int32_t>(startFrequency) +
                static_cast<int32_t>(delta * progress)
            );

        if (!setFrequency(frequency))
            return false;

        delay(STEP_DURATION_MS);
    }

    // Descending sweep
    for (uint16_t i = 0; i < halfSteps; ++i)
    {
        const float progress =
            static_cast<float>(i) / halfSteps;

        const int32_t delta =
            static_cast<int32_t>(endFrequency) -
            static_cast<int32_t>(peakFrequency);

        const uint32_t frequency =
            static_cast<uint32_t>(
                static_cast<int32_t>(peakFrequency) +
                static_cast<int32_t>(delta * progress)
            );

        if (!setFrequency(frequency))
            return false;

        delay(STEP_DURATION_MS);
    }

    ledcWrite(BUZZER_PIN, 0);
    return true;
}

}

namespace Buzzer
{

void begin()
{
    if (!ledcAttach(
            BUZZER_PIN,
            START_FREQUENCY_HZ,
            BUZZER_RESOLUTION))
    {
        Serial.println("ERROR: Could not attach buzzer to LEDC");
        return;
    }

    off();

    Serial.println("Buzzer initialized successfully");
}

void playDeterrentPattern()
{
    for (uint8_t i = 0; i < PATTERN_REPETITIONS; ++i)
    {
        if (!playSweep(
                START_FREQUENCY_HZ,
                PEAK_FREQUENCY_HZ,
                END_FREQUENCY_HZ))
        {
            off();
            return;
        }

        if (i < PATTERN_REPETITIONS - 1)
            delay(REPETITION_PAUSE_MS);
    }

    off();
}

void off()
{
    ledcWrite(BUZZER_PIN, 0);
}

}