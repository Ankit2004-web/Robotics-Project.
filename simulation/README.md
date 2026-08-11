# Simulation

## Status

A **Wokwi-oriented simulation package** is included under [`wokwi/`](wokwi/).

| Claim | Status |
|-------|--------|
| Simulation files created | YES |
| Perfect IR spectacles sensor model | NO (substituted input) |
| Physical optical eye detection | NOT SIMULATED |
| Useful for logic/timing checks | YES |

## What the simulator substitutes

The real project uses an IR blink sensor on spectacles. Most browser Arduino simulators do not model that optical setup accurately.

In Wokwi, eye closure is emulated with a **push button** on D2:

| Button state | Emulated meaning |
|--------------|------------------|
| Released (default HIGH with INPUT_PULLUP variant / or wired HIGH) | Configure to match firmware expectation |
| Held | Eye closed condition for threshold timing |

The provided Wokwi sketch uses the same thresholds and state machine as the hardware firmware, with comments explaining the substitute input.

## How to run (Wokwi)

1. Open [https://wokwi.com](https://wokwi.com)
2. Create/open an Arduino UNO project
3. Copy:
   - `wokwi/anti_sleep_alarm.ino` into the sketch
   - `wokwi/diagram.json` into the diagram/config
4. Start the simulation
5. Hold the button to emulate prolonged eye closure
6. Watch the buzzer and relay/motor indicators and Serial output

## Expected simulated timing

- Hold ≥ 3 s → warning buzzer path
- Hold ≥ 4 s → emergency stop path
- Release → reset to normal

## Limitations

- No real IR TX/RX optical path
- Relay/motor are representational loads
- Always validate final behavior on physical hardware
