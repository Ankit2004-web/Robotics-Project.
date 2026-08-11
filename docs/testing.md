# Testing

These tests match the **final firmware** thresholds:

- Warning: **3 seconds** eye closure → buzzer ON
- Emergency: **4 seconds** eye closure → motor stopped via relay
- Eye open / blink resume → reset to normal

## Preconditions

- Firmware uploaded to Arduino UNO
- Hardware wired per [`wiring.md`](wiring.md)
- 9V motor supply connected through the relay
- Serial Monitor optional at 115200 baud

## Test 1 — Normal blinking

**Procedure**

1. Power the Arduino and motor circuit.
2. Keep the sensor in the eye-open / normal blinking condition.

**Expected**

- Motor remains operational
- Buzzer remains inactive
- Serial state stays near `NORMAL` (or briefly enters `EYE_CLOSED` only during short blinks under 3 s)

## Test 2 — Warning threshold

**Procedure**

1. Force the sensor into the eye-closed condition.
2. Hold for about **3 seconds** (but less than 4).

**Expected**

- Buzzer activates
- Motor continues running
- State becomes `WARNING`

## Test 3 — Emergency stop threshold

**Procedure**

1. Keep the eye-closed condition beyond **4 seconds**.

**Expected**

- Buzzer remains ON
- Relay performs motor safety/stop action
- State becomes `EMERGENCY_STOP`

## Test 4 — Recovery after blinking resumes

**Procedure**

1. From warning or emergency state, restore eye-open / blink activity.

**Expected**

- Buzzer turns OFF
- Motor returns to running state
- State returns to `NORMAL`

## Test 5 — Relay polarity check

**Procedure**

1. With system in `NORMAL`, confirm the hobby motor is running.
2. Enter `EMERGENCY_STOP` and confirm the motor stops.
3. If behavior is inverted, set `RELAY_ACTIVE_LOW` to the opposite value and re-upload.

**Expected**

- Normal ⇒ motor ON
- Emergency ⇒ motor OFF

## Test 6 — Startup state

**Procedure**

1. Power-cycle the Arduino while the sensor indicates eye open.

**Expected**

- Boot enters normal outputs (motor allowed, buzzer off)
- No immediate false emergency stop

## Simulation Testing

If using the Wokwi project under `simulation/wokwi`:

- Use the documented substitute input (button/toggle) to emulate eye closed/open
- Verify the same 3 s / 4 s thresholds in the simulator Serial output

Simulation validates control logic timing. It does **not** fully reproduce the physical IR spectacles sensor optics.

## Result Logging Template

| Test | Result (Pass/Fail) | Notes |
|------|--------------------|-------|
| 1 Normal blinking | | |
| 2 Warning threshold | | |
| 3 Emergency stop | | |
| 4 Recovery | | |
| 5 Relay polarity | | |
| 6 Startup | | |

## Verification Status

Physical hardware execution of these tests must be performed on the real prototype. Packaging this repository does not by itself constitute a completed hardware validation run.
