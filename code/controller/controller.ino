#include "Arduino.h"
#include <EEPROM.h>
#include "MomentarySwitch.h"  //needs to be imported from shared code folder as .zip library
#include "IndicatorLED.h"

#define MODE_PIN 13   //physical pin 19
#define SENDING_MIDI 0
#define SWITCHING_TO_CHANGING_BANK 1
#define CHANGING_BANK 2
#define SWITCHING_TO_SENDING_MIDI 3
#define BANK_MEMORY_ADDRESS 0

void displayBinaryNumber(int number, int leds[]) {
  for (int i = 0; i < 5; i ++) {
    digitalWrite(leds[i], bitRead(number, i));
  }
}

void sendMidiProgramChange(int footswitch, int bank) {
  //C means signal is a program change, 0 means channel 1
  Serial.write(0xC0);
  //sends a maximum 7 bit int
  Serial.write(footswitch + bank * 5);

}

MomentarySwitch momentarySwitches[] = {
  MomentarySwitch(4), //physical pin 6
  MomentarySwitch(5), //physical pin 11
  MomentarySwitch(6), //physical pin 12
  MomentarySwitch(7), //physical pin 13
  MomentarySwitch(8)  //physical pin 14
};

//Analog pins are being used digitally
int leds[] = {
  A5, //physical pin 28
  A4, //physical pin 27
  A3, //physical pin 26
  A2, //physical pin 25
  A1  //physical pin 24
};    
IndicatorLED indicatorLED = IndicatorLED(12, 500);  //physical pin 18
int currentPreset;
int bank;
int state;


void setup () {
  Serial.begin(31250);      //MIDI baud rate
  pinMode(MODE_PIN, INPUT);
  for(int i = 0; i < 5; i++) {
    momentarySwitches[i].begin();
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  indicatorLED.begin();
  bank = EEPROM.read(BANK_MEMORY_ADDRESS);
  currentPreset = -1;
}

  
void loop() {
  switch(state) {
    case SENDING_MIDI:	
      if (digitalRead(MODE_PIN)) {
        state = SWITCHING_TO_CHANGING_BANK;
        break;
      }
            
      for(int i = 0; i < 5; i++) {
        if (momentarySwitches[i].update()) {
          sendMidiProgramChange(i, bank);
          if (currentPreset != -1) {
            digitalWrite(leds[currentPreset], LOW);
          }
          digitalWrite(leds[i], HIGH);
          currentPreset = i;
        }
      }
      break;

    case SWITCHING_TO_CHANGING_BANK:
      indicatorLED.activate();
      if (currentPreset != -1){
          digitalWrite(leds[currentPreset], LOW);
        }
      displayBinaryNumber(bank, leds);
      currentPreset = -1;
      state = CHANGING_BANK;
      
      break;
          
    case CHANGING_BANK:
      if(!digitalRead(MODE_PIN)) {
          state = SWITCHING_TO_SENDING_MIDI;
          break;
      }
            
      for(int i = 0; i < 5; i++) {
        //checks if a switch was pressed and if bank is < 25 after flipping the bit at i in bank
        if (momentarySwitches[i].update() && (bank ^ (1 << (4 - i))) < 25) {
          //flips the bit in bank for the footswitches binary number
          bank ^= (1 << (4 - i));
          displayBinaryNumber(bank, leds);
        }
      }
    	
      indicatorLED.update();
      break;
           
    case SWITCHING_TO_SENDING_MIDI:
      indicatorLED.deactivate();
      displayBinaryNumber(0, leds);
      EEPROM.write(BANK_MEMORY_ADDRESS, bank);
      state = SENDING_MIDI;
      break;
           
  }
}
           