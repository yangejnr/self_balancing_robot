# Self-Balancing Robot — Motor Driver Bring-Up

An early embedded-robotics workspace for a two-wheel self-balancing robot. The code currently focuses on validating the bidirectional motor-drive hardware before IMU feedback and closed-loop balancing are introduced.

## Current Scope

The present Arduino/ESP32 sketch is a **BTS7960 dual-motor direction test**, not yet a complete balancing controller. It provides serial commands for independently driving two motors forward/backward and stopping them.

```text
Serial Test Command
       │
       ▼
ESP32 / Controller
       │
   ┌───┴────┐
   ▼        ▼
BTS7960   BTS7960
   │        │
Left Motor Right Motor
```

## Implemented Test Commands

- `F1` — motor 1 forward
- `B1` — motor 1 backward
- `F2` — motor 2 forward
- `B2` — motor 2 backward
- `S` — stop both motors

## Why This Stage Matters

A balancing controller depends on predictable motor polarity and response. Separating motor-driver bring-up from IMU/PID development makes it easier to verify wiring and direction before closing the feedback loop.

## Planned Control Stack

Future development can extend this hardware baseline with:

- IMU angle/gyro measurement
- sensor filtering
- PID or state-feedback balancing control
- speed and steering commands
- safety cut-off on excessive tilt
- tuning and disturbance testing

## Status

**Hardware bring-up / early prototype.** The repository name describes the intended robot, while the current code specifically demonstrates motor-driver validation.

## Author

**Yange Henry Terzugwe**  
Robotics & Embedded Systems
