#include "Arduino.h"
#include "IndicatorLED.h"

IndicatorLED::IndicatorLED(int ledPin, int toggleDuration) {
  this->ledPin = ledPin;
  this->toggleDuration = toggleDuration;
  lastAction = 0;
  state = false;
}

void IndicatorLED::begin() {
  pinMode(ledPin, OUTPUT);
}

void IndicatorLED::activate() {
  state = true;
  digitalWrite(ledPin, state);
  lastAction = millis();
}

void IndicatorLED::deactivate() {
  state = false;
  digitalWrite(ledPin, state);
}

void IndicatorLED::update() {
  if (millis() - lastAction > toggleDuration) {
    state = !state;
    digitalWrite(ledPin, state);
    lastAction = millis();
  }
}