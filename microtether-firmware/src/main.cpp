#include <Arduino.h>

#include "patterns_generated.h"

#define V_MOTOR_MIN (0.75)
#define V_MOTOR_MAX (2.95)
#define V_LDO_OUT (3.30)

// ### SYTEM DEFINES
#define PINSTATE_VIBE_DISABLE (LOW)
#define ANALOG_RESOLUTION_BITS (8)
#define DEBOUNCE_MS (50)
#define SHUTDOWN_HOLD_TIME (2500) // ms
#define SERIALBAUD (9600)

// bounded input config
constexpr double intensityMin = 0.0; // low values later mapped to V_MOTOR_MIN, so 0.001 is not fully 'off'.
constexpr double intensityMax = 1.0;
constexpr double intensityStepSize = 0.2499;
constexpr double speedMin = 0.1;
constexpr double speedMax = 20.0;
constexpr double speedStepMultSize = 1.30; // can be more fine as we have + and - buttons.

// global vars
double global_intensity_value = 0.00; // start in off state
double global_speed_value = 1.5;
int global_pattern_value = 0;
constexpr int num_patterns = sizeof(loadedPatterns) / sizeof(void*);

// ### UTILS

inline double mapfloat(double x, double in_min, double in_max, double out_min, double out_max) {
double run = in_max - in_min;
  if (run == 0) {
    return NAN;
  }
  double rise = out_max - out_min;
  double delta = x - in_min;
  return (delta * rise) / run + out_min;
}

// #### OUTPUT PINS
constexpr uint8_t module_pins[] = {4, 5, 6}; // O1, O2, O3
constexpr uint8_t num_module_pins = sizeof(module_pins) / sizeof(uint8_t);

void disable_specific(uint8_t pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, PINSTATE_VIBE_DISABLE );
}

void config_and_disable_all_out() {
  for (int j = 0; j < num_module_pins; j++) {
    disable_specific(module_pins[j]);
  }
}

// #### INPUT PINS
// ORDER: cycle pattern, intensity cycle, speed+, speed-
constexpr uint8_t button_pins[] = {0, 1, 3, 2}; 
constexpr uint8_t num_button_pins = sizeof(button_pins) / sizeof(uint8_t);

void setup_buttons() {
  for (int j = 0; j < num_button_pins; j++) {
    pinMode(button_pins[j], INPUT_PULLUP);
  }
  delayMicroseconds(10); // yeep  
}

void printButtonStatuses() {
  Serial.print(digitalRead(button_pins[0]));
  Serial.print(digitalRead(button_pins[1]));
  Serial.print(digitalRead(button_pins[2]));
  Serial.print(digitalRead(button_pins[3]));
  Serial.println();
}

bool isAnyButtonPressed() {
  for (int j = 0; j < num_button_pins; j++) {
    if (!digitalRead(button_pins[j])) {
      return true;
    }
  }
  return false;
}
// #### Functionality

void write_out_safe(uint8_t pin, double value) {
  // respects 0.0 -> off
  if (value <= 0.0) {
    disable_specific(pin);
    return;
  }
  if (value > 1.0) {
    value = 1.0;
  }

  uint32_t pwm_absolute_integer_max = pow(2.0, ANALOG_RESOLUTION_BITS - 1);
  double value_ranged = mapfloat(value, 0.0, 1.0, V_MOTOR_MIN / V_LDO_OUT, V_MOTOR_MAX / V_LDO_OUT);
  analogWrite(pin, value_ranged * pwm_absolute_integer_max);
}

void do_button_polling() {
  static uint32_t time_of_last_button_press = 0;
  // control inputs' bounds set here:
  if (isAnyButtonPressed()) {
    Serial.println("BUTTON PRESSED");
    time_of_last_button_press = millis();

    if (!digitalRead(button_pins[0])) {
      // case is pattern+
      Serial.println("NEXT PATTERN");
      global_pattern_value += 1;
    }
    if (!digitalRead(button_pins[1])) {
      // case is intensity cycle
      Serial.println("INTENSITY CYCLE");
      global_intensity_value += intensityStepSize;
      if (global_intensity_value > intensityMax) {
        global_intensity_value = intensityMin;
      }
    }
    if (!digitalRead(button_pins[2])) {
      // case is speed+
      Serial.println("SPEED PLUS");
      global_speed_value *= speedStepMultSize;
      if (global_speed_value > speedMax) {
        global_speed_value = speedMax;
      }
    }
    if (!digitalRead(button_pins[3])) {
      // case is speed-
      Serial.println("SPEED MINUS");
      global_speed_value /= speedStepMultSize;
      if (global_speed_value < speedMin) {
        global_speed_value = speedMin;
      }
    }
    while (millis() - time_of_last_button_press < DEBOUNCE_MS) {
      delayMicroseconds(10); // yeep
    }
    while (isAnyButtonPressed()) {
      delayMicroseconds(10); // yeep
    }
  }

}

void startup_sequence_vibe() {
  const double startup_intensity = 0.7;
  const uint32_t startup_delays = 125;
  const uint8_t num_startup_pulses = 3;
  for (uint8_t x = 0; x < num_startup_pulses; x++) {
    for (int j = 0; j < num_module_pins; j++) {
      write_out_safe(module_pins[j], startup_intensity);
    }
    delay(startup_delays);
    config_and_disable_all_out();
    delay(startup_delays);
  }
}

void writeToChannelWithMult(uint8_t ch, double value) {
  write_out_safe(module_pins[(ch - 1) % num_module_pins], value * global_intensity_value);
}

void injectableDelayWithSpeedMultiplier(double t_s) {
  uint32_t t_start = millis();
  constexpr double MILLIS_TO_SECONDS  = 1000.0;
  while ((millis() - t_start) < uint32_t(t_s * MILLIS_TO_SECONDS * (1.0/global_speed_value))) {
    // inject superloop-ish polling functions here
    do_button_polling();
  }
}

//## terrible Serial control
void doSerialCommands() {
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case '+':
      case '=': {
        global_speed_value += 0.1;
        break;
      }
      case '-': {
        global_speed_value -= 0.1;
        break;
      }
      case 'p': {
        global_pattern_value++;
        break;
      }
      case 'i': {
        global_intensity_value += 0.1;
        break;
      }
      case 'u': {
        global_intensity_value -= 0.1;
        break;
      }
      default: {
        printButtonStatuses();
        break;
      }
    }
    Serial.print("S: ");
    Serial.println(global_speed_value);
    Serial.print("I: ");
    Serial.println(global_intensity_value);
    Serial.print("P: ");
    Serial.println(global_pattern_value);
    Serial.println();
  }
}
// ## main funcs
void setup() {
  Serial.begin(SERIALBAUD);
  pinMode(LED_BUILTIN, OUTPUT);
  analogWriteResolution(ANALOG_RESOLUTION_BITS);
  config_and_disable_all_out();
  setup_buttons();
  startup_sequence_vibe();
  Serial.println("SETUP ENDED");
}

void loop() {
  doSerialCommands();
  if (global_intensity_value > 0.00) {
    // this wont run if intensity is 0, effectively it will be OFF. this is our off switch
    loadedPatterns[global_pattern_value % num_patterns](writeToChannelWithMult, injectableDelayWithSpeedMultiplier); // run one cycle of current pattern
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  } else {
    digitalWrite(LED_BUILTIN, HIGH); // bro is inverted fsr
    config_and_disable_all_out();
    do_button_polling();
  }

}
