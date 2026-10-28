/*
  Fight the Landlord controller (Arduino Uno)
  VRx -> A4; VRy -> A5 (unused); joystick SW -> D2
  Separate mini button -> D3 and GND
  LEDs: You D4, Bot 1 D5, Bot 2 D6 (each through a 220-330 ohm resistor)
  Joystick VCC -> 5V; all GND connections -> Arduino GND.

  Tilt left/right: move the highlighted card (return to center each time).
  Click joystick straight down: select/deselect highlighted card.
  Press mini button: play selected cards, or pass if none are selected.
  During bidding: mini button bids the next available value.
  Other bidding choices use the on-screen buttons.
*/
#include <Arduino.h>
#include <string.h>

const byte PIN_JOY_X = A4;
const byte PIN_JOY_SW = 2;
const byte PIN_PLAY_BTN = 3;
const byte LED_P0 = 4;
const byte LED_P1 = 5;
const byte LED_P2 = 6;

// Reversed to match the orientation of your joystick.
// Set false if you later rotate or replace the joystick.
const bool REVERSE_X = true;
const int CENTER_MIN = 400;
const int CENTER_MAX = 620;
const int THRESHOLD_LOW = 250;
const int THRESHOLD_HIGH = 770;
const unsigned long DEBOUNCE_DELAY = 50;
bool movedX = false;

struct ButtonState {
  byte pin;
  bool lastReading;
  bool stableState;
  unsigned long changedAt;
};
ButtonState selectButton = {PIN_JOY_SW, HIGH, HIGH, 0};
ButtonState playButton = {PIN_PLAY_BTN, HIGH, HIGH, 0};

char incoming[24];
byte incomingLength = 0;
bool discardLine = false;
unsigned long lastTurnUpdate = 0;

void setTurnLEDs(int player) {
  digitalWrite(LED_P0, player == 0 ? HIGH : LOW);
  digitalWrite(LED_P1, player == 1 ? HIGH : LOW);
  digitalWrite(LED_P2, player == 2 ? HIGH : LOW);
}

void readButton(ButtonState &button, const char *command) {
  bool reading = digitalRead(button.pin);
  if (reading != button.lastReading) {
    button.changedAt = millis();
    button.lastReading = reading;
  }
  if (millis() - button.changedAt >= DEBOUNCE_DELAY &&
      reading != button.stableState) {
    button.stableState = reading;
    if (reading == LOW) Serial.println(command);
  }
}

void readJoystickInputs() {
  int x = analogRead(PIN_JOY_X);
  if (x >= CENTER_MIN && x <= CENTER_MAX) movedX = false;
  if (!movedX && (x < THRESHOLD_LOW || x > THRESHOLD_HIGH)) {
    bool left = (x < THRESHOLD_LOW);
    if (REVERSE_X) left = !left;
    Serial.println(left ? "NAV_LEFT" : "NAV_RIGHT");
    movedX = true;
  }
  // VRy is intentionally unused. Selection comes ONLY from the SW click.
}

void handleIncomingLine() {
  int player;
  if (strcmp(incoming, "TURN:0") == 0) player = 0;
  else if (strcmp(incoming, "TURN:1") == 0) player = 1;
  else if (strcmp(incoming, "TURN:2") == 0) player = 2;
  else if (strcmp(incoming, "TURN:OFF") == 0) player = -1;
  else return;
  setTurnLEDs(player);
  lastTurnUpdate = millis();
}

void readIncomingSerial() {
  // Consume complete lines without blocking joystick/button polling.
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (!discardLine) {
        incoming[incomingLength] = '\0';
        handleIncomingLine();
      }
      incomingLength = 0;
      discardLine = false;
    } else if (!discardLine) {
      if (incomingLength < sizeof(incoming) - 1) incoming[incomingLength++] = c;
      else discardLine = true;
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_JOY_SW, INPUT_PULLUP);
  pinMode(PIN_PLAY_BTN, INPUT_PULLUP);
  pinMode(LED_P0, OUTPUT);
  pinMode(LED_P1, OUTPUT);
  pinMode(LED_P2, OUTPUT);
  digitalWrite(LED_P0, HIGH);
  digitalWrite(LED_P1, HIGH);
  digitalWrite(LED_P2, HIGH);
  delay(500);
  setTurnLEDs(-1);
  Serial.println("READY");
}

void loop() {
  readJoystickInputs();
  readButton(selectButton, "TOGGLE_SELECT");
  readButton(playButton, "BTN_PLAY");
  readIncomingSerial();
  // Turn lights off if the game stops sending its one-second heartbeat.
  if (millis() - lastTurnUpdate > 3500) setTurnLEDs(-1);
}
