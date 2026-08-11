# Troubleshooting

## Sensor always reads closed or always open

**Possible causes**

- Incorrect VCC/GND/OUT wiring
- Sensor sensitivity pot not adjusted
- Spectacles alignment not facing the eye region
- `EYE_CLOSED_LEVEL` does not match your module

**What to do**

1. Open Serial Monitor (115200) and watch state changes.
2. Manually cover/uncover the IR path and confirm transitions.
3. Adjust the sensor potentiometer if present.
4. If your module is inverted from the documentation, change `EYE_CLOSED_LEVEL`.

## Buzzer never sounds

- Confirm D12 wiring and ground
- Confirm you are holding eye-closed ≥ 3 seconds
- Measure/observe D12 going HIGH in warning state
- Verify buzzer type (some passive buzzers need tone; this project expects a simple HIGH/LOW driven buzzer as in the original build)

## Motor never runs

- Confirm 9V battery is charged and connected
- Confirm COM/NC motor wiring
- Confirm relay VCC/GND/IN connections
- Check whether relay polarity is inverted (`RELAY_ACTIVE_LOW`)
- Ensure you are not expecting Arduino GPIO to power the motor directly

## Motor runs when it should stop (or opposite)

This is the most common relay-module mismatch.

1. Note current `RELAY_ACTIVE_LOW` value in firmware
2. Invert it
3. Re-upload
4. Repeat Test 5 in [`testing.md`](testing.md)

## Alarm triggers too easily

- Increase `SENSOR_DEBOUNCE_MS`
- Increase `WARNING_THRESHOLD_MS` / `STOP_THRESHOLD_MS`
- Improve sensor mechanical stability on the spectacles
- Reduce ambient IR interference if possible

## Alarm is too slow

- Decrease thresholds in firmware constants
- Remember final defaults are 3 s warning / 4 s stop to match the original code/AIM

## Arduino not detected / upload fails

- Install correct USB-serial drivers for your UNO/cable
- Select the right board and COM port
- Try another USB cable (data-capable)
- Close Serial Monitor during upload if needed

## System resets unexpectedly

- Motor electrical noise / supply dips
- Weak USB power while relay coil switches
- Missing common grounds on the control side
- Separate motor supply recommended (already part of design)

## Serial shows nothing

- Set baud rate to **115200**
- Confirm `SERIAL_DEBUG` is `true`
- Reselect the COM port after upload

## Safety reminder

If anything smells hot, vibrates abnormally, or wiring is uncertain:

1. Disconnect power immediately
2. Recheck motor/relay connections
3. Do not connect this prototype to a real vehicle braking system
