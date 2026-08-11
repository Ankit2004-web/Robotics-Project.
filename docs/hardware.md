# Hardware

## Overview

This project is a low-voltage **Arduino UNO** prototype of an anti-sleep alarm concept. An IR blink sensor mounted on transparent spectacles monitors eye state. Prolonged eye closure triggers a piezo warning and then stops a hobby motor through a relay.

## Components

| Component | Purpose |
|-----------|---------|
| Arduino UNO | Embedded controller |
| Arduino IDE | Firmware development / upload |
| IR blink sensor | Detects eye open/closed state via IR TX/RX |
| Transparent spectacles | Mechanical mount for the blink sensor |
| Piezo buzzer | Audible warning to the driver/operator |
| Relay module | Isolates and switches the motor load |
| Hobby motor + wheel | Demonstrates autonomous wheel motion / stop |
| 9V battery | Motor power supply |
| Jumper wires | Interconnects |

## Blink Sensor

The blink sensor consists of:

- an **IR transmitter**
- an **IR receiver**

mounted on transparent spectacles so the optical path interacts with the eye region.

Documented digital behavior:

| Eye condition | Sensor output |
|---------------|---------------|
| Eye shut | HIGH |
| Eye open / normal blink activity | LOW |

## Relay and Motor

The relay is required because a hobby motor can draw more current than an Arduino GPIO can safely supply.

Documented load wiring uses:

- relay **COM**
- relay **NC** (Normally Closed)
- **9V battery** as the motor supply

Intended demonstration behavior:

| Condition | Motor |
|-----------|-------|
| Normal monitoring | Running (circuit closed through NC) |
| Emergency threshold exceeded | Stopped (relay opens NC path) |

## Electrical Safety Boundaries

- Logic side: Arduino 5V peripherals
- Load side: 9V battery + motor through relay
- This is a **prototype demonstration**, not a vehicle braking system

## Bill of Materials (Prototype)

| Item | Qty | Notes |
|------|-----|-------|
| Arduino UNO | 1 | Or compatible board |
| IR blink / eye sensor module | 1 | 3-pin: VCC, GND, OUT |
| Piezo buzzer | 1 | Active or passive as used in original build |
| 5V relay module | 1 | Single-channel preferred |
| DC hobby gear motor | 1 | With wheel for demo |
| 9V battery + snap connector | 1 | Motor supply |
| Spectacles mount | 1 | Transparent glasses used in original |
| Jumper wires | as needed | Male-male / male-female |

## What Was Not Part of the Original Project

Do **not** treat these as original hardware unless clearly marked optional:

- Camera / OpenCV drowsiness detection
- Real automotive ECU integration
- Hydraulic / mechanical vehicle brakes
- GPS, GSM, cloud telemetry
- Production-grade fail-safe redundant controllers
