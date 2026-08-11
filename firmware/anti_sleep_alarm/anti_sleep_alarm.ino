/**
 * Anti Sleep Alarm – Autonomous Wheel Robot
 *
 * Arduino UNO firmware for an IR blink-sensor prototype that:
 *  1) Monitors prolonged eye closure
 *  2) Sounds a piezo buzzer after a warning threshold
 *  3) Stops a hobby motor via relay after an emergency threshold
 *
 * Source of truth: academic project documentation by Ankit Biswas (22052533)
 * and team. This firmware preserves the intended safety behavior while
 * replacing the original blocking loop with a non-blocking state machine.
 *
 * Pin map (from documentation):
 *  - Blink sensor OUT  -> D2
 *  - Piezo buzzer (+)  -> D12
 *  - Relay IN          -> D13
 *
 * IMPORTANT: Prototype only. Not certified for real-vehicle use.
 */

// ---------------------------------------------------------------------------
// Pin configuration
// ---------------------------------------------------------------------------
const uint8_t BLINK_SENSOR_PIN = 2;
const uint8_t BUZZER_PIN       = 12;
const uint8_t MOTOR_RELAY_PIN  = 13;

// ---------------------------------------------------------------------------
// Timing thresholds (milliseconds)
// ---------------------------------------------------------------------------
// Original documentation:
//  - Theory: buzzer ~3 s, brakes ~5 s
//  - Aim / original code: stop at ~4 s
// Final implementation matches the original Arduino code + AIM statement.
const unsigned long WARNING_THRESHOLD_MS = 3000UL;  // activate buzzer
const unsigned long STOP_THRESHOLD_MS    = 4000UL;  // stop motor via relay

// Ignore very short LOW/HIGH glitches from the IR sensor.
const unsigned long SENSOR_DEBOUNCE_MS = 50UL;

// ---------------------------------------------------------------------------
// Hardware polarity configuration
// ---------------------------------------------------------------------------
// Theory: eye shut => sensor output HIGH.
const bool EYE_CLOSED_LEVEL = HIGH;

// Buzzer is active HIGH in the original implementation.
const bool BUZZER_ACTIVE_LEVEL = HIGH;

/**
 * Relay polarity (critical):
 *
 * Documented motor circuit uses relay COM + NC with a 9V battery.
 * Original code behavior:
 *  - Normal / motor running  => digitalWrite(motorPin, LOW)
 *  - Emergency stop          => digitalWrite(motorPin, HIGH)
 *
 * With NC wiring, that means writing HIGH energizes the relay and opens the
 * motor path (ACTIVE HIGH control from the sketch's point of view).
 *
 * Many common Songle-style modules are ACTIVE LOW. If your module stops the
 * motor when it should run (or vice versa), set RELAY_ACTIVE_LOW = true.
 */
const bool RELAY_ACTIVE_LOW = false;

// Optional Serial diagnostics (115200 baud).
const bool SERIAL_DEBUG = true;

// ---------------------------------------------------------------------------
// State machine
// ---------------------------------------------------------------------------
enum SystemState : uint8_t {
  STATE_NORMAL = 0,
  STATE_EYE_CLOSED,
  STATE_WARNING,
  STATE_EMERGENCY_STOP
};

SystemState currentState = STATE_NORMAL;

bool eyeClosedStable = false;
bool lastRawEyeClosed = false;
unsigned long lastSensorChangeMs = 0;
unsigned long eyeClosedSinceMs = 0;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
void setBuzzer(bool on) {
  const bool level = on ? BUZZER_ACTIVE_LEVEL : !BUZZER_ACTIVE_LEVEL;
  digitalWrite(BUZZER_PIN, level ? HIGH : LOW);
}

void setMotorRunning(bool running) {
  // running=true  => motor ON  (relay de-energized for NC wiring + active-high)
  // running=false => motor OFF (relay energized)
  bool energizeRelay = !running;
  bool pinLevel;

  if (RELAY_ACTIVE_LOW) {
    pinLevel = energizeRelay ? LOW : HIGH;
  } else {
    pinLevel = energizeRelay ? HIGH : LOW;
  }

  digitalWrite(MOTOR_RELAY_PIN, pinLevel);
}

void motorOn() {
  setMotorRunning(true);
}

void motorOff() {
  setMotorRunning(false);
}

void activateWarning() {
  setBuzzer(true);
}

void activateMotorStop() {
  motorOff();
  setBuzzer(true);
}

void resetAlarm() {
  setBuzzer(false);
  motorOn();
  currentState = STATE_NORMAL;
  eyeClosedSinceMs = 0;
}

const char* stateName(SystemState state) {
  switch (state) {
    case STATE_NORMAL:         return "NORMAL";
    case STATE_EYE_CLOSED:     return "EYE_CLOSED";
    case STATE_WARNING:        return "WARNING";
    case STATE_EMERGENCY_STOP: return "EMERGENCY_STOP";
    default:                   return "UNKNOWN";
  }
}

void debugState(SystemState previous, SystemState next, unsigned long closedMs) {
  if (!SERIAL_DEBUG) {
    return;
  }
  Serial.print(F("[state] "));
  Serial.print(stateName(previous));
  Serial.print(F(" -> "));
  Serial.print(stateName(next));
  Serial.print(F(" | eyeClosedMs="));
  Serial.println(closedMs);
}

bool readEyeClosedRaw() {
  const int value = digitalRead(BLINK_SENSOR_PIN);
  return value == (EYE_CLOSED_LEVEL ? HIGH : LOW);
}

/**
 * Debounced eye-closed reading.
 * Returns true when the eye appears closed (sensor reports EYE_CLOSED_LEVEL).
 */
bool updateEyeClosedSensor(unsigned long nowMs) {
  const bool rawClosed = readEyeClosedRaw();

  if (rawClosed != lastRawEyeClosed) {
    lastRawEyeClosed = rawClosed;
    lastSensorChangeMs = nowMs;
  }

  if ((nowMs - lastSensorChangeMs) >= SENSOR_DEBOUNCE_MS) {
    eyeClosedStable = rawClosed;
  }

  return eyeClosedStable;
}

void enterState(SystemState next, unsigned long closedMs) {
  if (next == currentState) {
    return;
  }
  const SystemState previous = currentState;
  currentState = next;
  debugState(previous, next, closedMs);
}

void handleNormalState(bool eyeClosed, unsigned long nowMs) {
  if (!eyeClosed) {
    // Keep outputs in the safe normal configuration.
    setBuzzer(false);
    motorOn();
    return;
  }

  eyeClosedSinceMs = nowMs;
  enterState(STATE_EYE_CLOSED, 0);
}

void handleDrowsiness(bool eyeClosed, unsigned long nowMs) {
  if (!eyeClosed) {
    resetAlarm();
    if (SERIAL_DEBUG) {
      Serial.println(F("[event] eye opened -> reset to NORMAL"));
    }
    return;
  }

  const unsigned long closedMs = nowMs - eyeClosedSinceMs;

  if (closedMs >= STOP_THRESHOLD_MS) {
    activateMotorStop();
    enterState(STATE_EMERGENCY_STOP, closedMs);
    return;
  }

  if (closedMs >= WARNING_THRESHOLD_MS) {
    activateWarning();
    // Motor remains running during warning.
    motorOn();
    enterState(STATE_WARNING, closedMs);
    return;
  }

  // Closed, but still below warning threshold.
  setBuzzer(false);
  motorOn();
  enterState(STATE_EYE_CLOSED, closedMs);
}

void setup() {
  pinMode(BLINK_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_RELAY_PIN, OUTPUT);

  // Startup-safe state: motor allowed to run, buzzer off.
  // Matches original sketch intent (motor enabled at boot).
  resetAlarm();

  lastRawEyeClosed = readEyeClosedRaw();
  eyeClosedStable = lastRawEyeClosed;
  lastSensorChangeMs = millis();

  if (SERIAL_DEBUG) {
    Serial.begin(115200);
    Serial.println();
    Serial.println(F("Anti Sleep Alarm – Autonomous Wheel Robot"));
    Serial.println(F("Firmware ready."));
    Serial.print(F("WARNING_THRESHOLD_MS="));
    Serial.println(WARNING_THRESHOLD_MS);
    Serial.print(F("STOP_THRESHOLD_MS="));
    Serial.println(STOP_THRESHOLD_MS);
    Serial.print(F("RELAY_ACTIVE_LOW="));
    Serial.println(RELAY_ACTIVE_LOW ? F("true") : F("false"));
    Serial.print(F("EYE_CLOSED_LEVEL="));
    Serial.println(EYE_CLOSED_LEVEL ? F("HIGH") : F("LOW"));
  }
}

void loop() {
  const unsigned long nowMs = millis();
  const bool eyeClosed = updateEyeClosedSensor(nowMs);

  switch (currentState) {
    case STATE_NORMAL:
      handleNormalState(eyeClosed, nowMs);
      break;

    case STATE_EYE_CLOSED:
    case STATE_WARNING:
    case STATE_EMERGENCY_STOP:
      handleDrowsiness(eyeClosed, nowMs);
      break;
  }
}
