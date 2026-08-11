/**
 * Wokwi simulation sketch for Anti Sleep Alarm logic timing.
 *
 * IMPORTANT:
 * - This is a SUBSTITUTE for the physical IR blink sensor.
 * - A push button on D2 emulates "eye closed" when pressed (pin reads LOW).
 * - Therefore this simulation sketch inverts the hardware eye-closed level.
 *
 * Hardware firmware (firmware/anti_sleep_alarm/anti_sleep_alarm.ino):
 *   eye closed = HIGH
 *
 * This Wokwi sketch:
 *   eye closed = button pressed = LOW on D2 (with INPUT_PULLUP)
 */

const uint8_t BLINK_SENSOR_PIN = 2;
const uint8_t BUZZER_PIN = 12;
const uint8_t MOTOR_RELAY_PIN = 13;

const unsigned long WARNING_THRESHOLD_MS = 3000UL;
const unsigned long STOP_THRESHOLD_MS = 4000UL;
const unsigned long SENSOR_DEBOUNCE_MS = 50UL;

// In Wokwi button emulation, pressed => LOW => treat as eye closed.
const bool EYE_CLOSED_LEVEL = LOW;
const bool RELAY_ACTIVE_LOW = false;

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

void setBuzzer(bool on) {
  digitalWrite(BUZZER_PIN, on ? HIGH : LOW);
}

void setMotorRunning(bool running) {
  bool energizeRelay = !running;
  bool pinLevel = RELAY_ACTIVE_LOW ? (energizeRelay ? LOW : HIGH)
                                   : (energizeRelay ? HIGH : LOW);
  digitalWrite(MOTOR_RELAY_PIN, pinLevel);
}

void resetAlarm() {
  setBuzzer(false);
  setMotorRunning(true);
  currentState = STATE_NORMAL;
  eyeClosedSinceMs = 0;
}

bool updateEyeClosedSensor(unsigned long nowMs) {
  const bool rawClosed = digitalRead(BLINK_SENSOR_PIN) == (EYE_CLOSED_LEVEL ? HIGH : LOW);
  if (rawClosed != lastRawEyeClosed) {
    lastRawEyeClosed = rawClosed;
    lastSensorChangeMs = nowMs;
  }
  if ((nowMs - lastSensorChangeMs) >= SENSOR_DEBOUNCE_MS) {
    eyeClosedStable = rawClosed;
  }
  return eyeClosedStable;
}

const char* stateName(SystemState state) {
  switch (state) {
    case STATE_NORMAL: return "NORMAL";
    case STATE_EYE_CLOSED: return "EYE_CLOSED";
    case STATE_WARNING: return "WARNING";
    case STATE_EMERGENCY_STOP: return "EMERGENCY_STOP";
    default: return "UNKNOWN";
  }
}

void setup() {
  pinMode(BLINK_SENSOR_PIN, INPUT_PULLUP);  // button to GND
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_RELAY_PIN, OUTPUT);
  resetAlarm();
  lastRawEyeClosed = digitalRead(BLINK_SENSOR_PIN) == LOW;
  eyeClosedStable = lastRawEyeClosed;
  lastSensorChangeMs = millis();

  Serial.begin(115200);
  Serial.println(F("Wokwi Anti Sleep Alarm simulation"));
  Serial.println(F("Hold button = eye closed"));
}

void loop() {
  const unsigned long nowMs = millis();
  const bool eyeClosed = updateEyeClosedSensor(nowMs);
  SystemState next = currentState;

  if (!eyeClosed) {
    if (currentState != STATE_NORMAL) {
      resetAlarm();
      Serial.println(F("eye opened -> NORMAL"));
    } else {
      setBuzzer(false);
      setMotorRunning(true);
    }
    return;
  }

  if (currentState == STATE_NORMAL) {
    eyeClosedSinceMs = nowMs;
    next = STATE_EYE_CLOSED;
  }

  const unsigned long closedMs = nowMs - eyeClosedSinceMs;
  if (closedMs >= STOP_THRESHOLD_MS) {
    setBuzzer(true);
    setMotorRunning(false);
    next = STATE_EMERGENCY_STOP;
  } else if (closedMs >= WARNING_THRESHOLD_MS) {
    setBuzzer(true);
    setMotorRunning(true);
    next = STATE_WARNING;
  } else {
    setBuzzer(false);
    setMotorRunning(true);
    next = STATE_EYE_CLOSED;
  }

  if (next != currentState) {
    Serial.print(stateName(currentState));
    Serial.print(F(" -> "));
    Serial.print(stateName(next));
    Serial.print(F(" @ "));
    Serial.print(closedMs);
    Serial.println(F(" ms"));
    currentState = next;
  }
}
