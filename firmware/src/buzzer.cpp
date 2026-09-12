#include "buzzer.h"

#include <Arduino.h>

namespace
{
constexpr uint8_t BUZZER_PIN = 23;
constexpr uint8_t BUZZER_RESOLUTION = 1;

constexpr uint32_t START_FREQUENCY_HZ = 2700;
constexpr uint32_t PEAK_FREQUENCY_HZ = 3100;
constexpr uint32_t END_FREQUENCY_HZ = 2500;

constexpr uint16_t SWEEP_DURATION_MS = 750;
constexpr uint16_t STEP_DURATION_MS = 10;

constexpr uint8_t PATTERN_REPETITIONS = 3;
constexpr uint16_t REPETITION_PAUSE_MS = 40;

void playSweep(
    uint32_t startFrequency,
    uint32_t peakFrequency,
    uint32_t endFrequency)
{
    const uint16_t steps =
        SWEEP_DURATION_MS / STEP_DURATION_MS;

    const uint16_t halfSteps = steps / 2;

    for (uint16_t i = 0; i < halfSteps; ++i)
    {
        const float progress =
            static_cast<float>(i) / halfSteps;

        const uint32_t frequency =
            startFrequency +
            static_cast<uint32_t>(
                (peakFrequency - startFrequency) * progress);

        ledcWriteTone(BUZZER_PIN, frequency);

        delay(STEP_DURATION_MS);
    }

    for (uint16_t i = 0; i < halfSteps; ++i)
    {
        const float progress =
            static_cast<float>(i) / halfSteps;

        const uint32_t frequency =
            peakFrequency +
            static_cast<uint32_t>(
                (endFrequency - peakFrequency) * progress);

        ledcWriteTone(BUZZER_PIN, frequency);

        delay(STEP_DURATION_MS);
    }

    ledcWriteTone(BUZZER_PIN, 0);
}
}

namespace Buzzer
{
void begin()
{
    ledcAttach(
        BUZZER_PIN,
        START_FREQUENCY_HZ,
        BUZZER_RESOLUTION);

    off();
}

void playDeterrentPattern()
{
    for (uint8_t i = 0; i < PATTERN_REPETITIONS; ++i)
    {
        playSweep(
            START_FREQUENCY_HZ,
            PEAK_FREQUENCY_HZ,
            END_FREQUENCY_HZ);

        if (i < PATTERN_REPETITIONS - 1)
        {
            delay(REPETITION_PAUSE_MS);
        }
    }
}

void off()
{
    ledcWrite(BUZZER_PIN, 0);
}
}