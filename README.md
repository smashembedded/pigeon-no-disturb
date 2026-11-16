# Pigeon No Disturb

An ESP32-based smart pigeon detection and non-harmful deterrence system for outdoor terrace environments.

## Overview

Pigeon No Disturb is an experimental embedded system designed to detect pigeon activity in a monitored area, trigger a non-harmful deterrent response, and record events for later analysis.

The project is being developed incrementally, starting with a simple ESP32 and ultrasonic sensor prototype before progressively adding deterrence, autonomous operation, monitoring, and behavioural evaluation.

The initial prototype is based on:

- ESP32 Dev Module
- HC-SR04 ultrasonic distance sensor
- Buzzer-based deterrent
- Battery-powered operation (planned)
- Local Wi-Fi dashboard (planned)

The project focuses not only on detecting pigeons, but also on evaluating whether the deterrent produces a measurable change in their behaviour.

## Project Goals

- Detect activity in a defined outdoor area.
- Trigger a non-harmful deterrent response.
- Record detection and deterrent events.
- Monitor the system through a local Wi-Fi dashboard.
- Evaluate deterrent effectiveness using real-world observations.

## Hardware

The current prototype uses:

| Component | Description |
|---|---|
| ESP32 Dev Module | Main microcontroller |
| HC-SR04 | Ultrasonic distance sensor |
| Passive buzzer | Acoustic deterrent |
| Voltage divider | Reduces the HC-SR04 ECHO voltage |
| USB connection | Power and firmware upload during development |

The following components are planned for later stages:

- LED status indicator
- Push button
- 4×AA battery supply
- Outdoor enclosure

## Wiring and Connections

### ESP32 pin assignment

| Component | Pin | ESP32 connection |
|---|---|---|
| HC-SR04 | VCC | 5V |
| HC-SR04 | GND | GND |
| HC-SR04 | TRIG | GPIO 5 |
| HC-SR04 | ECHO | GPIO 18, through a voltage divider |
| Buzzer | Signal (+) | GPIO 23 |
| Buzzer | GND (-) | GND |

**Important:** The HC-SR04 ECHO output is approximately 5 V, while ESP32 GPIO pins operate at 3.3 V. A voltage divider must be used to protect the ESP32.

### HC-SR04 ECHO voltage divider

The current prototype uses two resistors:

- 1 kΩ in series between ECHO and the GPIO connection.
- 2 kΩ between the GPIO connection and GND.

Connection:

```text
HC-SR04 ECHO
     |
    1 kΩ
     |
     +---------- ESP32 GPIO 18
     |
    2 kΩ
     |
    GND
```

This reduces the nominal 5 V ECHO signal to approximately 3.3 V.

All components must share a common GND.

**Power note:** During initial development, the ESP32 is powered through USB. The 4×AA battery supply is part of a later development stage and should not be assumed to be fully integrated yet.

## Software Requirements

The firmware is developed using:

- PlatformIO
- Arduino framework for ESP32
- ESP32 Dev Module (`esp32dev`)

The project currently targets:

| Package | Version |
|---|---|
| PlatformIO Espressif32 platform | 6.7.0 |
| Arduino-ESP32 framework | 3.0.2 |
| Board | `esp32dev` |

The firmware uses the Arduino-ESP32 3.x LEDC API.

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/smashembedded/pigeon-no-disturb.git
cd pigeon-no-disturb
```

### 2. Install PlatformIO

Install PlatformIO Core:

```bash
python -m pip install platformio
```

Alternatively, install the PlatformIO IDE extension in Visual Studio Code.

### 3. Connect the ESP32

Connect the ESP32 Dev Module to your computer using a USB cable.

Check that the board is detected:

```bash
pio device list
```

On Linux, the serial device will typically be something like:

```text
/dev/ttyUSB0
```

or:

```text
/dev/ttyACM0
```

The actual device name may vary.

### 4. Build the firmware

From the project root directory, run:

```bash
pio run
```

This compiles the firmware without uploading it.

### 5. Upload the firmware

To upload the firmware:

```bash
pio run -t upload
```

If PlatformIO cannot identify the serial port automatically, specify it explicitly:

```bash
pio run -t upload --upload-port /dev/ttyUSB0
```

Replace `/dev/ttyUSB0` with the port identified on your computer.

### 6. Open the serial monitor

Once the firmware has been uploaded, open the serial monitor:

```bash
pio device monitor
```

The monitor runs at **115200 baud**.

You can also specify the port:

```bash
pio device monitor --port /dev/ttyUSB0
```

To close the monitor, press `Ctrl+C`.

### 7. Test the system

With the HC-SR04 connected:

1. Upload and start the firmware.
2. Open the serial monitor.
3. Place an object in front of the ultrasonic sensor.
4. Move the object closer than the configured detection threshold.
5. Check the distance measurements and detection events.
6. Move the object away and check the departure event.

The current detection threshold is 30 cm.

The buzzer deterrent is triggered by a confirmed presence in the current prototype integration.

## Detection Model

The current firmware converts filtered ultrasonic measurements into detection events using a state machine:

```text
CLEAR → CONFIRMING → PRESENT → CLEAR
```

### States

- **CLEAR** — No object is currently detected within the configured detection range.
- **CONFIRMING** — A possible presence has been detected and consecutive measurements are being checked to reduce false triggers.
- **PRESENT** — Presence has been confirmed.

The current detection threshold is **30 cm**.

A presence is confirmed after **3 consecutive filtered measurements** within the detection threshold.

When a presence is confirmed, the firmware generates:

```text
Event: PRESENCE_DETECTED
```

When the detected object leaves the detection range, the firmware generates:

```text
Event: DEPARTED | Presence duration: <duration> ms
```

The presence duration is measured from the moment the presence is confirmed until departure is detected.

### Distance Measurement

The HC-SR04 measurements are filtered before being passed to the detection system.

The current implementation:

- Takes 5 ultrasonic measurements for each filtered reading.
- Discards invalid measurements.
- Accepts measurements between 2 cm and 400 cm.
- Calculates the average of the valid measurements.
- Returns `-1` when no valid measurement is available.

Invalid measurements do not represent a real distance and are therefore ignored by the detection logic.

### Detection Behaviour

The detector uses the following rules:

- Distance **≤ 30 cm** → object detected.
- Distance **> 30 cm** → object not detected.
- Invalid measurement → detection state is not changed.

The confirmation mechanism requires three consecutive valid detections before generating `PRESENCE_DETECTED`. This helps reduce false triggers caused by individual erroneous measurements.

Once presence has been confirmed, the system remains in the `PRESENT` state until a valid measurement indicates that the object has left the detection range.

Every new confirmed presence is treated as a new detection. The system does not attempt to determine whether a returning object is the same pigeon previously detected, since the ultrasonic sensor cannot reliably identify individual animals.

Cooldown behaviour is not currently part of the detection layer. Any deterrent cooldown will be implemented separately from the detection system so that detection events remain available for later behavioural analysis.

## Acoustic Deterrent

The first deterrent prototype uses a passive buzzer connected to GPIO 23.

The current sound pattern is designed to resemble a short hawk-like call:

- Starting frequency: 2700 Hz.
- Peak frequency: 3100 Hz.
- Ending frequency: 2500 Hz.
- Sweep duration: 750 ms.
- Repetitions: 3.
- Pause between repetitions: 40 ms.

The pattern is implemented in `buzzer.cpp`.

**Current limitation:** The frequency sweep is still under validation. The current implementation produces LEDC frequency-configuration errors on the ESP32. This issue is being investigated and the acoustic pattern should be considered experimental until the fix is validated.

## Development

The project is being developed incrementally through several stages:

1. **Detection** — ESP32 + HC-SR04 distance measurement and presence detection.
2. **Deterrence** — Non-harmful buzzer response.
3. **Autonomous device** — Outdoor enclosure and battery-powered operation.
4. **Monitoring** — Local Wi-Fi dashboard.
5. **Experimentation** — Measurement of deterrent effectiveness.
6. **Field testing** — Validation in a real terrace environment.

## Current Status

Implemented:

- ESP32 and HC-SR04 integration.
- Ultrasonic distance measurement.
- Distance filtering and invalid measurement handling.
- Configurable presence threshold.
- Presence confirmation using a state machine.
- Departure detection and presence duration measurement.
- Initial buzzer control and hawk-like acoustic pattern.

Under development:

- LEDC frequency-sweep correction and acoustic validation.
- Configurable deterrent duration and cooldown.
- Outdoor enclosure.
- Battery-powered operation.
- Local Wi-Fi dashboard.
- Behavioural experiments and field testing.

## Repository Structure

```text
pigeon-no-disturb/
├── firmware/
│   ├── include/
│   │   ├── buzzer.h
│   │   ├── detection.h
│   │   └── ultrasonic.h
│   └── src/
│       ├── buzzer.cpp
│       ├── detection.cpp
│       ├── main.cpp
│       └── ultrasonic.cpp
├── .gitignore
├── LICENSE
├── platformio.ini
└── README.md
```

## License

See the [LICENSE](LICENSE) file for details.