# Pigeon No Disturb

An ESP32-based smart pigeon detection and non-harmful deterrence system for outdoor terrace environments.

## Overview

Pigeon No Disturb is an experimental embedded system designed to detect pigeon activity in a monitored area, trigger a non-harmful deterrent response, and record events for later analysis.

The project is being developed incrementally, starting with a simple ESP32 and ultrasonic sensor prototype before progressively adding deterrence, autonomous operation, monitoring, and behavioural evaluation.

The initial prototype is based on:

* ESP32
* HC-SR04 ultrasonic sensor
* Buzzer-based deterrent
* Battery power
* Local Wi-Fi dashboard

The project focuses not only on detecting pigeons, but also on evaluating whether the deterrent produces a measurable change in their behaviour.

## Project Goals

* Detect activity in a defined outdoor area.
* Trigger a non-harmful deterrent response.
* Record detection and deterrent events.
* Monitor the system through a local Wi-Fi dashboard.
* Evaluate deterrent effectiveness using real-world observations.

## Detection Model

The current firmware converts filtered ultrasonic measurements into detection events using a simple state machine:

```text
CLEAR → CONFIRMING → PRESENT → CLEAR
```

### States

* **CLEAR** — No object is currently detected within the configured detection range.
* **CONFIRMING** — A possible presence has been detected and consecutive measurements are being checked to reduce false triggers.
* **PRESENT** — Presence has been confirmed.

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

* Takes 5 ultrasonic measurements for each filtered reading.
* Discards invalid measurements.
* Accepts measurements between 2 cm and 400 cm.
* Calculates the average of the valid measurements.
* Returns `-1` when no valid measurement is available.

Invalid measurements do not represent a real distance and are therefore ignored by the detection logic.

### Detection Behaviour

The detector uses the following rules:

* Distance **≤ 30 cm** → object detected.
* Distance **> 30 cm** → object not detected.
* Invalid measurement → detection state is not changed.

The confirmation mechanism requires three consecutive valid detections before generating `PRESENCE_DETECTED`. This helps reduce false triggers caused by individual erroneous measurements.

Once presence has been confirmed, the system remains in the `PRESENT` state until a valid measurement indicates that the object has left the detection range.

Every new confirmed presence is treated as a new detection. The system does not attempt to determine whether a returning object is the same pigeon previously detected, since the ultrasonic sensor cannot reliably identify individual animals.

Cooldown behaviour is not currently part of the detection layer. Any deterrent cooldown will be implemented separately from the detection system so that detection events remain available for later behavioural analysis.

## Hardware

The current prototype uses:

* ESP32 Dev Module
* HC-SR04 ultrasonic distance sensor
* Voltage divider for the HC-SR04 ECHO signal
* Buzzer
* LED
* Push button
* 4×AA battery supply

The HC-SR04 ECHO output is reduced to a safe voltage level before being connected to the ESP32 GPIO.

## Development

The project is being developed incrementally through several stages:

1. **Detection** — ESP32 + HC-SR04 distance measurement and presence detection.
2. **Deterrence** — Non-harmful buzzer response.
3. **Autonomous device** — Outdoor enclosure and battery-powered operation.
4. **Monitoring** — Local Wi-Fi dashboard.
5. **Experimentation** — Measurement of deterrent effectiveness.
6. **Field testing** — Validation in a real terrace environment.

## Status

Early prototype — ultrasonic distance measurement, filtering, threshold-based detection, presence confirmation, departure detection, and presence duration measurement are implemented.

The deterrent, autonomous operation, local dashboard, and behavioural experiments are planned for subsequent development stages.

## Repository Structure

```text
pigeon-no-disturb/
├── firmware/
│   ├── include/
│   │   ├── detection.h
│   │   └── ultrasonic.h
│   └── src/
│       ├── detection.cpp
│       ├── main.cpp
│       └── ultrasonic.cpp
├── LICENSE
├── platformio.ini
└── README.md
```
