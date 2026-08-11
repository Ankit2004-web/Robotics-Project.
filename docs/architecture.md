# System Architecture

## High-Level Architecture

```text
Blink Sensor (IR TX/RX on spectacles)
              │
              ▼
         Arduino UNO
     Drowsiness Detection
      (state machine)
         │         │
         ▼         ▼
      Buzzer     Relay
                   │
                   ▼
              Hobby Motor
           (9V battery load)
```

## Functional Blocks

| Block | Role |
|-------|------|
| Sensing | IR blink sensor reports eye open/closed |
| Control | Arduino evaluates closure duration |
| Warning | Piezo buzzer alerts the operator |
| Actuation | Relay interrupts motor power for demo stop |
| Power | Arduino logic supply + separate 9V motor supply |

## Control Flow

1. Arduino initializes outputs to a normal state (motor allowed, buzzer off).
2. Firmware continuously reads the blink sensor (debounced).
3. If the eye remains closed:
   - ≥ 3 s → warning buzzer
   - ≥ 4 s → relay stops the hobby motor
4. If the eye opens / blinking resumes, the system resets to normal.

## State Model

| State | Entry condition | Outputs |
|-------|-----------------|---------|
| `NORMAL` | Eye open | Motor ON, buzzer OFF |
| `EYE_CLOSED` | Closure detected, < 3 s | Motor ON, buzzer OFF |
| `WARNING` | Closure ≥ 3 s and < 4 s | Motor ON, buzzer ON |
| `EMERGENCY_STOP` | Closure ≥ 4 s | Motor OFF, buzzer ON |

## Hardware/Software Boundary

| Concern | Owner |
|---------|-------|
| Eye-state sensing | Hardware IR module + D2 input |
| Timing thresholds | Firmware constants |
| Audible alert | Firmware → D12 → buzzer |
| Motor power switching | Firmware → D13 → relay → 9V motor circuit |

## Design Constraints

- Prototype-scale demonstration only
- No claim of automotive safety certification
- Motor current must remain on the relay/battery side
- Sensor substitute may be used in simulation, but is not identical to the physical IR spectacles setup

## Diagrams

- Architecture (GitHub-friendly PNG): [`../diagrams/system-architecture.png`](../diagrams/system-architecture.png)
- Architecture (SVG): [`../diagrams/system-architecture.svg`](../diagrams/system-architecture.svg)
- Circuit (PNG): [`../diagrams/circuit-diagram.png`](../diagrams/circuit-diagram.png)
- Flowchart (PNG): [`../diagrams/system-flowchart.png`](../diagrams/system-flowchart.png)
- Original scanned circuit photo: [`../media/original-circuit-diagram.png`](../media/original-circuit-diagram.png)
