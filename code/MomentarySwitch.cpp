#include "Arduino.h"
#include "MomentarySwitch.h"

MomentarySwitch::MomentarySwitch(int readPin, int debounceDelay = 50) {
  this->readPin = readPin;
  this->debounceDelay = debounceDelay;
  lastAction = 0;
}

void MomentarySwitch::begin() {
  pinMode(readPin, INPUT);
}

bool MomentarySwitch::update() {
  if (digitalRead(readPin) != state && millis() - lastAction > debounceDelay) {
    state = !state;
    lastAction = millis();
    return state;
  }
  else
    return false;
}