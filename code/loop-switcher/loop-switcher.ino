#include <MIDI.h>
#include <EEPROM.h>
#include "MomentarySwitch.h"

#define CURRENT_PRESET_ADDRESS 125

MIDI_CREATE_DEFAULT_INSTANCE();

const int relayPins[] = {A2, A1, A0, 13, 12, 11, 10, 9};
MomentarySwitch buttons[] = {
  MomentarySwitch(A5),
  MomentarySwitch(A4),
  MomentarySwitch(A3),
  MomentarySwitch(4),
  MomentarySwitch(5),
  MomentarySwitch(6),
  MomentarySwitch(7),
  MomentarySwitch(8)
};

byte presets[125];

int currentPreset;

void readPresets(byte presets[]) {
  for(int i = 0; i < 125; i++) {
    presets[i] = EEPROM.read(i);
  }
}


void setRelays(byte channel, byte number) {
  for (int i = 0; i < 8; i ++) {
    digitalWrite(relayPins[i], bitRead(presets[number], i));
  }
  EEPROM.write(CURRENT_PRESET_ADDRESS, number);
  currentPreset = number;
}

void updateRelay(int relay) {
  presets[currentPreset] ^= (1 << relay);
  digitalWrite(relayPins[relay], bitRead(presets[currentPreset], relay));
  EEPROM.write(currentPreset, presets[currentPreset]);

};


void setup() {
  MIDI.begin();
  MIDI.setHandleProgramChange(setRelays);
  for (MomentarySwitch button : buttons) {
    button.begin();
  }
  
  readPresets(presets);
  currentPreset = EEPROM.read(CURRENT_PRESET_ADDRESS);

  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], bitRead(presets[currentPreset], i));
  }
}

void loop() {
  MIDI.read();
  for(int i = 0; i < 8; i++) {
    if (buttons[i].update()){
      updateRelay(i);
    }
  }

}