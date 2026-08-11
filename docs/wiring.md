# Wiring Guide

This document describes the Arduino UNO pinout and power connections for the Anti Sleep Alarm autonomous wheel robot prototype, based on the original project documentation and circuit diagram.

## Power Domains

Keep these domains conceptually separate:

| Domain | Voltage | Purpose |
|--------|---------|---------|
| Arduino logic | 5 V (USB or regulated) | Blink sensor, buzzer, relay coil/control |
| Motor load | 9 V battery | Hobby motor switched through the relay |

The relay isolates the motor current from Arduino GPIO pins. **Do not power the hobby motor directly from an Arduino digital pin.**

## Pinout Table

| Component | Arduino Pin | Power | Ground | Purpose |
|-----------|-------------|-------|--------|---------|
| Blink sensor (OUT) | D2 | 5V | GND | Eye open/closed digital signal |
| Piezo buzzer (+) | D12 | — | GND | Audible drowsiness warning |
| Relay module (IN) | D13 | 5V (VCC) | GND | Switches motor circuit |
| Relay module (VCC) | — | Arduino 5V | — | Relay module logic/coil supply |
| Relay module (GND) | — | — | Arduino GND | Common ground with Arduino |
| Hobby motor | — | via relay + 9V battery | via battery/relay path | Demonstrates vehicle motion/stop |
| 9V battery | — | Motor circuit only | Tied as documented below | Motor power source |

## Detailed Connections

### 1. Blink Sensor (IR transmitter + IR receiver module)

| Sensor Lead | Connects To |
|-------------|-------------|
| VCC | Arduino **5V** |
| GND | Arduino **GND** |
| OUT / Signal | Arduino **Digital Pin 2** |

Documented sensor behavior:

- Eye **shut** → sensor output **HIGH**
- Eye **open / blinking normally** → sensor output **LOW**

### 2. Piezo Buzzer

| Buzzer Lead | Connects To |
|-------------|-------------|
| Signal / (+) | Arduino **Digital Pin 12** |
| GND / (−) | Arduino **GND** |

Active level in firmware: **HIGH = ON**.

### 3. Relay Module (control side)

| Relay Pin | Connects To |
|-----------|-------------|
| VCC | Arduino **5V** |
| GND | Arduino **GND** |
| IN | Arduino **Digital Pin 13** |

### 4. Motor + Battery (load side)

From the original procedure:

1. Connect battery **anode (+)** to relay **COM**.
2. Connect one motor lead to relay **NC** (Normally Closed).
3. Connect the other motor lead to battery **cathode (−)**.

Equivalent path when the relay is idle (not energized):

```text
9V Battery (+) → Relay COM → Relay NC → Hobby Motor → Battery (−)
```

When the relay is energized, the COM–NC path opens and the motor stops.

> Note: Some photos/procedures can look ambiguous between COM/NC/NO labeling. The written procedure specifies **NC**. Firmware defaults assume that writing **HIGH** to D13 produces the stop action used by the original sketch. If your module polarity differs, change `RELAY_ACTIVE_LOW` in the `.ino` file.

## Grounding Notes

- Arduino GND is shared with the blink sensor, buzzer, and relay module control ground.
- The 9V motor circuit returns through the battery negative via the relay/motor path.
- Avoid accidental shorts between motor supply rails and Arduino signal pins.

## Quick Checklist

- [ ] Sensor on D2, 5V, GND
- [ ] Buzzer on D12 and GND
- [ ] Relay IN on D13, VCC on 5V, GND on GND
- [ ] Motor powered by 9V through relay COM/NC
- [ ] No motor load on Arduino GPIO
- [ ] Polarity verified before powering the motor circuit
