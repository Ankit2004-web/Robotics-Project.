# Software

## Firmware Location

```text
firmware/anti_sleep_alarm/anti_sleep_alarm.ino
```

## Intended Behavior

| Eye condition duration | System response |
|------------------------|-----------------|
| Open / normal blinking | Motor ON, buzzer OFF |
| Closed ≥ 3 seconds | Buzzer ON (warning), motor still ON |
| Closed ≥ 4 seconds | Motor OFF via relay (emergency stop), buzzer remains ON |
| Eye opens again | Reset to normal operation |

These thresholds match the original Arduino implementation and project AIM. The written theory also mentioned ~5 seconds for braking; that inconsistency is documented below.

## Original Implementation (Documented)

The academic report included logic equivalent to:

```cpp
const int blinkPin = 2;
const int motorPin = 13;
const int buzzerPin = 12;

long time;

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(blinkPin, INPUT);
  digitalWrite(motorPin, HIGH);
}

void loop() {
  if (!digitalRead(blinkPin)) {
    time = millis();
    while (!digitalRead(blinkPin)) {
      digitalWrite(buzzerPin, LOW);
      digitalWrite(motorPin, LOW);
      delay(1000);
    }
  } else {
    if (TimeDelay() >= 3) digitalWrite(buzzerPin, HIGH);
    if (TimeDelay() >= 4) digitalWrite(motorPin, HIGH);
  }
}

int TimeDelay() {
  long t = millis() - time;
  t = t / 1000;
  return t;
}
```

### Issues in the Original Code

| Issue | Effect |
|-------|--------|
| Uninitialized `time` | If the sensor starts HIGH, delay math is undefined |
| Blocking `while` + `delay(1000)` | Loop responsiveness reduced; timing coarse |
| Theory vs code timing | Theory ~5 s stop; code/AIM ~4 s stop |
| Raw pin writes for motor | Easy to misread whether HIGH means motor ON or relay ON |
| No debounce | IR noise can create false transitions |
| No explicit state model | Harder to reason about warning vs emergency |

### Original Output Semantics

| Condition | `buzzerPin` | `motorPin` |
|-----------|-------------|------------|
| Eye open path | LOW | LOW |
| Warning (≥3 s closed) | HIGH | LOW (still) |
| Stop (≥4 s closed) | HIGH | HIGH |

Because the motor load uses the relay **NC** path, `motorPin HIGH` corresponds to the **stop** action in the documented build.

## Improved Implementation

The current firmware uses a non-blocking state machine:

```text
NORMAL → EYE_CLOSED → WARNING → EMERGENCY_STOP
              ↑_______________________|
                     (eye opens)
```

### Key Design Choices

- `millis()`-based timing (no blocking wait for drowsiness timing)
- Named constants for pins and thresholds
- Debounced sensor input
- `motorOn()` / `motorOff()` helpers
- Configurable `RELAY_ACTIVE_LOW`
- Optional Serial debug at 115200 baud
- Deterministic reset when blinking/eye-open resumes

### Configuration Constants

| Constant | Default | Meaning |
|----------|---------|---------|
| `WARNING_THRESHOLD_MS` | 3000 | Buzzer activation |
| `STOP_THRESHOLD_MS` | 4000 | Motor stop via relay |
| `SENSOR_DEBOUNCE_MS` | 50 | Glitch rejection |
| `EYE_CLOSED_LEVEL` | HIGH | Matches theory |
| `RELAY_ACTIVE_LOW` | false | Matches original HIGH=stop semantics |
| `SERIAL_DEBUG` | true | Prints state transitions |

## Engineering Improvements

| Original | Improved | Reason |
|----------|----------|--------|
| Blocking `while` + `delay(1000)` | Non-blocking state machine | Keeps control loop responsive |
| Magic numbers `3` / `4` seconds | Named millisecond thresholds | Clearer and easier to tune |
| Direct motor pin HIGH/LOW meaning unclear | `motorOn()` / `motorOff()` + polarity flag | Safer maintenance across relay modules |
| No debounce | 50 ms debounce | Reduces false triggers |
| Uninitialized timer | Timer starts when closure is detected | Deterministic timing |
| Implicit behavior | Explicit states + Serial logs | Easier testing and demos |
| `setup()` wrote `motorPin HIGH` before first open-eye path | Startup calls `resetAlarm()` (motor ON, buzzer OFF) | Aligns boot state with intended normal operation |

## Build / Upload

1. Install Arduino IDE
2. Open `firmware/anti_sleep_alarm/anti_sleep_alarm.ino`
3. Board: **Arduino UNO**
4. Select the correct COM port
5. Upload
6. Open Serial Monitor at **115200** if debug is enabled

## Verification Status

| Item | Status |
|------|--------|
| Logic rewritten from documentation | IMPLEMENTED |
| Static review of state machine | DOCUMENTED |
| Physical upload/runtime test in this environment | REQUIRES PHYSICAL HARDWARE TESTING |
| Arduino CLI compilation in this environment | Not available on the authoring machine at packaging time |
