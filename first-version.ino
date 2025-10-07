#include <MIDI.h>
#include <EEPROM.h>

const int relayPins[] = {2, 3, 4, 5, 6, 7};
const int footswitchPin = 14;
const int relayButtonPin = 15;
const int debounceDelay = 10;

int footswitchPinValue;
int relayButtonPinValue;
int currentPreset;

unsigned long footswitchLastPressed;
unsigned long relayButtonLastPressed;

bool presets[5][6];
bool debouncingFootswitch = false;
bool debouncingRelayButton = false;

MIDI_CREATE_DEFAULT_INSTANCE();

void readPresetsFromEEPROM(bool presets[5][6]){
  int address = 0;
  for (int i = 0; i < 5; i++) {
    for (int k = 0; k < 6; k++) {
      presets[i][k] = EEPROM.read(address);
      address++;
    }
  }
}

int setToPreset(int analogReading, bool presets[5][6]) {
    int preset;
    
    if (analogReading > 1000) {
      preset = 0;
    }
    else if (analogReading > 900) {
      preset = 1;
    }
    else if (analogReading > 700) {
      preset = 2;
    }
    else if (analogReading > 600) {
      preset = 3;
    }
    else {
      preset = 4;
    }
    
    for(int i = 0; i < 6; i++) {
      digitalWrite(relayPins[i], presets[preset][i]);
    }

    MIDI.sendProgramChange(preset, 1);

    return preset;
}

void toggleRelayAndSavePreset(int analogReading, bool presets[5][6], int currentPreset) {
      int button;
      if (relayButtonPinValue > 1000) {
        button = 0;
      }
      else if (relayButtonPinValue > 800) {
        button = 1;
      }
      else if (relayButtonPinValue > 700) {
        button = 2;
      }
      else if (relayButtonPinValue > 500) {
        button = 3;
      }
      else if (relayButtonPinValue > 400) {
        button = 4;
      }
      else {
        button = 5;
      }

      bool newState = !presets[currentPreset][button];
      presets[currentPreset][button] = newState;
      EEPROM.write(currentPreset * 6 + button, newState);
      digitalWrite(relayPins[button], newState);
}

void setup() {
  readPresetsFromEEPROM(presets);
  currentPreset = 0;
  setToPreset(1023, presets);
  
  MIDI.begin(1);
  
  for(int i = 0; i < 8; i++) {
    pinMode(i, OUTPUT);
  }
  
}

void loop() {
  //handles footswitch input and changes preset
  footswitchPinValue = analogRead(footswitchPin);
  if (footswitchPinValue > 50) {
    if (!debouncingFootswitch) {
      footswitchLastPressed = millis();
      debouncingFootswitch = true;
      
    }
    else if (millis() - footswitchLastPressed > debounceDelay) {
      currentPreset = setToPreset(analogRead(footswitchPin), presets);
      debouncingFootswitch = false;
      delay(250);
    }
  }
  
  //handles relay button input and writing new preset
  relayButtonPinValue = analogRead(relayButtonPin);
  if (relayButtonPinValue > 50) {
    if(!debouncingRelayButton) {
      relayButtonLastPressed = millis();
      debouncingRelayButton = true;
    }
    else if (millis() - relayButtonLastPressed > debounceDelay) {
      toggleRelayAndSavePreset(analogRead(relayButtonPin), presets, currentPreset);
      debouncingRelayButton = false;
      delay(250);
    }
  }
}